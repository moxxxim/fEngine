#include <Feng/ScenesManager/MeshRenderer.h>

#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Light.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/ScenesManager/RenderProperties.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Mesh.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/ResourcesManager/Texture.h>
#include <Feng/Utils/Render/ShaderParams.h>
#include <Feng/Utils/Render/RenderUtils.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    namespace SMeshRenderer
    {
        Matrix4 GetCamViewProjInverse(const RenderProperties &renderProperties, const std::pair<float, float>& nearFar)
        {
            const float camFov = renderProperties.cam->GetFovY();
            const float camAspect = renderProperties.cam->GetAspectRatio();
            const Matrix4 camTransform = renderProperties.cam->GetEntity()->GetComponent<Transform>()->GetGlobalMatrix();
            const Matrix4 projection = Mat4::MakePerspectiveProjection(camFov, camAspect, nearFar.first, nearFar.second);
            Matrix4 projectionInverted;
            std::ignore = projection.TryInvert(projectionInverted);
            
            return projectionInverted * camTransform;
        }
        
        Matrix4 GetShadowCastLightProjectionMatrix(
                                                   const RenderProperties &renderProperties,
                                                   const Matrix4& lightViewMatrix,
                                                   const std::pair<float, float>& nearFar)
        {
            Matrix4 camViewProjInverse = GetCamViewProjInverse(renderProperties, nearFar);
            std::array<Vector4, 8> camFrustum = Render::GetFrustumXyzMinMax(camViewProjInverse);

            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::lowest();
            float minY = std::numeric_limits<float>::max();
            float maxY = std::numeric_limits<float>::lowest();
            float minZ = std::numeric_limits<float>::max();
            float maxZ = std::numeric_limits<float>::lowest();
            for(const Vector4& corner : camFrustum)
            {
                const Vector4 cornerInLightSpace = corner * lightViewMatrix;
                minX = std::min(minX, cornerInLightSpace.x);
                maxX = std::max(maxX, cornerInLightSpace.x);
                minY = std::min(minY, cornerInLightSpace.y);
                maxY = std::max(maxY, cornerInLightSpace.y);
                minZ = std::min(minZ, cornerInLightSpace.z);
                maxZ = std::max(maxZ, cornerInLightSpace.z);
            }

            minZ -= 10.f;
            maxZ += 20.f;

            return Mat4::MakeOrthogonalProjection(minX, maxX, minY, maxY, minZ, maxZ);
        }
        
        Matrix4 GetShadowCastLightViewMatrix(const Entity* light)
        {
            const Transform *lightTransform = light->GetComponent<Transform>();
            return Mat4::MakeTransformation(Vector3::One, Vector3::Zero, lightTransform->GetRotation().Inverse());
        }
 
        std::vector<std::pair<float, float>> CalculateCascadesNeraFar(const RenderProperties &renderProperties)
        {
            std::vector<std::pair<float, float>> cascadesNearFar;
            
            float minNear = renderProperties.cam->GetNearClipPlane();
            float maxFar = renderProperties.cam->GetFarClipPlane();
            float near = minNear;
            for(int i = 0; i < renderProperties.cascadeBorders.size(); ++i)
            {
                float cascadeBorder = renderProperties.cascadeBorders[i];
                float far = minNear + (maxFar - minNear) * cascadeBorder;
                cascadesNearFar.push_back(std::make_pair(near, far));
                near = far;
            }
            
            cascadesNearFar.push_back(std::make_pair(near, maxFar));

            return cascadesNearFar;
        }
        
        std::vector<Matrix4> GetDirectShadowMatrices(const RenderProperties &renderProperties)
        {
            std::vector<Matrix4> cascadeMatrices;

            std::vector<std::pair<float, float>> cascadesNearFar = CalculateCascadesNeraFar(renderProperties);
            Matrix4 lightViewMatrix = GetShadowCastLightViewMatrix(renderProperties.directShadowLight);
            for(const std::pair<float, float>& nearFar : cascadesNearFar)
            {
                Matrix4 lightProjection = GetShadowCastLightProjectionMatrix(renderProperties, lightViewMatrix, nearFar);
                cascadeMatrices.push_back(lightViewMatrix * lightProjection);
            }
            
            return cascadeMatrices;
        }
        
        std::vector<Matrix4> GetOmnidirectionalShadowLightMatrices(const Entity* lightEntity)
        {
            std::vector<Matrix4> matrices;
            
            const Light* light = lightEntity->GetComponent<Light>();
            const Matrix4 lightProjection = Mat4::MakePerspectiveProjection(90.f, 1.f, 1.f, light->GetRange());
            const Vector3& lightPos = lightEntity->GetComponent<Transform>()->GetPosition();
            
            Matrix4 transformPosX;
            Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneZ, 180.f} * Quaternion{Vector3::OneY, -90.f}).TryInvert(transformPosX);
            Matrix4 transformNegX;
            Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneZ, 180.f} * Quaternion{Vector3::OneY, 90.f}).TryInvert(transformNegX);
            matrices.push_back(transformPosX * lightProjection);
            matrices.push_back(transformNegX * lightProjection);
            
            Matrix4 transformPosY;
            Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneX, 90.f}).TryInvert(transformPosY);
            Matrix4 transformNegY;
            Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneX, -90.f}).TryInvert(transformNegY);
            matrices.push_back(transformPosY * lightProjection);
            matrices.push_back(transformNegY * lightProjection);
            
            Matrix4 transformPosZ;
            Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneZ, 180.f} * Quaternion{Vector3::OneY, 180.f}).TryInvert(transformPosZ);
            Matrix4 transformNegZ;
            Mat4::MakeTransformation(Vector3::One, lightPos, Quaternion{Vector3::OneZ, 180.f}).TryInvert(transformNegZ);
            matrices.push_back(transformPosZ * lightProjection);
            matrices.push_back(transformNegZ * lightProjection);

            return matrices;
        }
    }
    
    MeshRenderer::~MeshRenderer()
    {
        DeleteGeomertyBuffers();
        DeleteTextureBuffers();
    }

    void MeshRenderer::SetMaterial(Material *aMaterial)
    {
        if (material != aMaterial)
        {
            material = aMaterial;
            DeleteTextureBuffers();
            if(material)
            {
                CreateTexturesBuffers();
            }
        }
    }

    bool MeshRenderer::IsShadowCaster() const
    {
        return shadowCaster;
    }
    
    void MeshRenderer::SetShadowCaster(bool aShadowCaster)
    {
        shadowCaster = aShadowCaster;
    }
    
    void MeshRenderer::SetMesh(Mesh *aMesh)
    {
        if (mesh != aMesh)
        {
            if(mesh)
            {
                DeleteGeomertyBuffers();
            }

            mesh = aMesh;
            if(mesh)
            {
                CreateGeometryBuffers();
            }
        }
    }

    void MeshRenderer::SetInstanceTransforms(const std::vector<Matrix4>& instances)
    {
        if(static_cast<uint32_t>(instances.size()) > instancesCount)
        {
            if(instancesCount > 0)
            {
                DeleteInstanceBuffer();
            }
            
            CreateInstanceBuffer();
            Print_Errors_OpengGL();
        }

        UpdateInstanceBuffer(instances);
        Print_Errors_OpengGL();

        instancesCount = static_cast<uint32_t>(instances.size());
    }

    void MeshRenderer::AddExternalTexture(const std::string_view& name, eTextureType type, int32_t bufferId)
    {
        externalTextures[name] = std::make_pair(type, bufferId);
    }
    
    void MeshRenderer::RemoveExternalTexture(const std::string_view& name)
    {
        externalTextures.erase(name);
    }
    
    void MeshRenderer::Draw(const RenderProperties &renderProperties, bool isShadowPass, Material *externalMaterial)
    {
        Material* workingMaterial = externalMaterial ? externalMaterial : material;
        
        if (mesh && workingMaterial)
        {
            std::map<std::string, uint32_t> externalTextureBuffers;
            if(externalMaterial)
            {
                externalTextureBuffers = Render::CreateTextureBuffers(*externalMaterial);
            }
            
            std::map<std::string, uint32_t> *workingTextures = externalMaterial
                ? &externalTextureBuffers
                : &textureBuffers;
            
            StartDraw(*workingMaterial);
            Print_Errors_OpengGL();
            SetGlobalUniforms(renderProperties, *workingMaterial);
            Print_Errors_OpengGL();
            Render::ResolveBindings(*workingMaterial->GetShader(), workingMaterial->Bindings(), *workingTextures);
            BindExternalTextures(static_cast<uint32_t>(workingTextures->size()), *workingMaterial);
            Print_Errors_OpengGL();
            
            ExecuteDraw();
            FinishDraw();
            Print_Errors_OpengGL();
            
            for(auto&& [name, tbo] : externalTextureBuffers)
            {
                glDeleteTextures(1, &tbo);
            }
        }
    }

    void MeshRenderer::StartDraw(Material &workingMaterial)
    {
        workingMaterial.Apply();
        Print_Errors_OpengGL();
    }

    void MeshRenderer::FinishDraw()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
        Print_Errors_OpengGL();
    }

    void MeshRenderer::SetGlobalUniforms(const RenderProperties &renderProperties, Material &workingMaterial)
    {
        if(instancesCount == 0)
        {
            const Shader *shader = workingMaterial.GetShader();
            const Transform *myTransform = GetEntity()->GetComponent<Transform>();
            shader->SetUniformMatrix4(ShaderParams::ModelMatrix.data(), myTransform->GetGlobalMatrix());
            Print_Errors_OpengGL();
        }

        SetCameraUniforms(renderProperties, workingMaterial);
        SetLightUniforms(renderProperties, workingMaterial);
        SetShadowLightUniform(renderProperties, workingMaterial);
    }

    void MeshRenderer::ExecuteDraw()
    {
        glBindVertexArray(vertexBuffer.vao);

        ePrimitiveType primitiveType = mesh->GetPrimitiveType();

        // No need to call 'glBindBuffer' because it was bound while VAO was still bound.

        if(vertexBuffer.ibo != Render::UndefinedBuffer)
        {
            const std::vector<uint32_t>& indices = mesh->GetIndices();
            if(instancesCount > 0)
            {
                glDrawElementsInstanced(
                               ToOpenGLValue(primitiveType),
                               indices.size(),
                               GL_UNSIGNED_INT,
                               reinterpret_cast<void*>(0),
                               instancesCount);
            }
            else
            {
                glDrawElements(
                               ToOpenGLValue(primitiveType),
                               indices.size(),
                               GL_UNSIGNED_INT,
                               reinterpret_cast<void*>(0));
            }
        }
        else
        {
            int32_t verticesCount = mesh->GetVerticesCount();
            if(instancesCount > 0)
            {
                glDrawArraysInstanced(ToOpenGLValue(primitiveType), 0, verticesCount, instancesCount);
            }
            else
            {
                glDrawArrays(ToOpenGLValue(primitiveType), 0, verticesCount);
            }
        }

        glBindVertexArray(Render::UndefinedBuffer);
        Print_Errors_OpengGL();
    }

    void MeshRenderer::CreateGeometryBuffers()
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        vertexBuffer.vao = vao;
        vertexBuffer.vbo = CreateVertexBuffer();

        if(mesh->HasIndices())
        {
            vertexBuffer.ibo = CreateIndexBuffer();
        }

        glBindVertexArray(Render::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::UndefinedBuffer);
    }

    void MeshRenderer::DeleteGeomertyBuffers()
    {
        glDeleteBuffers(1, &vertexBuffer.vbo);
        glDeleteBuffers(1, &vertexBuffer.ibo);
        glDeleteBuffers(1, &vertexBuffer.vao);
        glDeleteBuffers(1, &instancesBuffer);

        vertexBuffer.vao = vertexBuffer.vbo = vertexBuffer.ibo = instancesBuffer = Render::UndefinedBuffer;
    }

    void MeshRenderer::CreateTexturesBuffers()
    {
        textureBuffers = Render::CreateTextureBuffers(*material);
    }

    void MeshRenderer::DeleteTextureBuffers()
    {
        for(const auto& [name, tbo] : textureBuffers)
        {
            glDeleteTextures(1, &tbo);
        }
    }

    void MeshRenderer::CreateInstanceBuffer()
    {
        glGenBuffers(1, &instancesBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, instancesBuffer);
        glBindVertexArray(vertexBuffer.vao);

        glEnableVertexAttribArray(firstInstanceAttributeIndex);
        glVertexAttribPointer(firstInstanceAttributeIndex, sizeof(float), GL_FLOAT, GL_FALSE,
                              sizeof(Matrix4), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(firstInstanceAttributeIndex + 1);
        glVertexAttribPointer(firstInstanceAttributeIndex + 1, sizeof(float), GL_FLOAT, GL_FALSE,
                              sizeof(Matrix4), reinterpret_cast<void*>(sizeof(Vector4)));
        glEnableVertexAttribArray(firstInstanceAttributeIndex + 2);
        glVertexAttribPointer(firstInstanceAttributeIndex + 2, sizeof(float), GL_FLOAT, GL_FALSE,
                              sizeof(Matrix4), reinterpret_cast<void*>(2 * sizeof(Vector4)));
        glEnableVertexAttribArray(firstInstanceAttributeIndex + 3);
        glVertexAttribPointer(firstInstanceAttributeIndex + 3, sizeof(float), GL_FLOAT, GL_FALSE,
                              sizeof(Matrix4), reinterpret_cast<void*>(3 * sizeof(Vector4)));

        glVertexAttribDivisor(firstInstanceAttributeIndex, 1);
        glVertexAttribDivisor(firstInstanceAttributeIndex + 1, 1);
        glVertexAttribDivisor(firstInstanceAttributeIndex + 2, 1);
        glVertexAttribDivisor(firstInstanceAttributeIndex + 3, 1);

        glBindVertexArray(Render::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
    }

    void MeshRenderer::UpdateInstanceBuffer(const std::vector<Matrix4>& instances)
    {
        if(!instances.empty())
        {
            glBindBuffer(GL_ARRAY_BUFFER, instancesBuffer);
            Print_Errors_OpengGL();
            glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(Matrix4), instances.data(), GL_STATIC_DRAW);
            Print_Errors_OpengGL();
            glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
        }
    }

    void MeshRenderer::DeleteInstanceBuffer()
    {
        glDeleteBuffers(1, &instancesBuffer);
    }

    uint32_t MeshRenderer::CreateVertexBuffer()
    {
        uint32_t bufferObject;
        glGenBuffers(1, &bufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
        glBufferData(GL_ARRAY_BUFFER, mesh->GetDataSize(), mesh->GetData(), GL_STATIC_DRAW);

        firstInstanceAttributeIndex = Render::EnableVertexAttributes(mesh->GetAttributes());

        return bufferObject;
    }

    uint32_t MeshRenderer::CreateIndexBuffer()
    {
        const std::vector<uint32_t>& indices = mesh->GetIndices();

        GLuint bufferObject;
        glGenBuffers(1, &bufferObject);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        return bufferObject;
    }
    
    void MeshRenderer::BindExternalTextures(uint32_t firstTextureUnit, Material &workingMaterial)
    {
        uint32_t textureUnit = firstTextureUnit;
        for(auto&& [name, typeAndId] : externalTextures)
        {
            uint32_t bufferId = typeAndId.second;
            if(bufferId != Render::UndefinedBuffer)
            {
                Shader *shader = workingMaterial.GetShader();
                Render::BindTexture(*shader, typeAndId.first, textureUnit, name, bufferId);
                ++textureUnit;
            }
        }
    }

    void MeshRenderer::SetCameraUniforms(const RenderProperties &renderProperties, Material &workingMaterial)
    {
        const Shader *shader = workingMaterial.GetShader();
        
        shader->SetUniformFloat(ShaderParams::NearClipPlane.data(), renderProperties.cam->GetNearClipPlane());
        shader->SetUniformFloat(ShaderParams::FarClipPlane.data(), renderProperties.cam->GetFarClipPlane());
        shader->SetUniformBuffer(ShaderParams::CamUniformBlock.data(), RenderProperties::CamBufferBinding);
        Print_Errors_OpengGL();
    }

    void MeshRenderer::SetLightUniforms(const RenderProperties &renderProperties, Material &workingMaterial)
    {
        const Shader *shader = workingMaterial.GetShader();
        shader->SetUniformVector4(ShaderParams::AmbientColor.data(), renderProperties.ambientColorAndIntencity);

        const Transform *pointLightTransform = renderProperties.pointLight->GetEntity()->GetComponent<Transform>();
        Vector4 pointLightPositionAndRange = pointLightTransform->GetPosition();
        pointLightPositionAndRange.w = renderProperties.pointLight->GetRange();
        Vector4 pointLightColor = renderProperties.pointLight->GetColor();
        pointLightColor.w = renderProperties.pointLight->GetIntesity();

        shader->SetUniformVector4(ShaderParams::PointLightColor.data(), pointLightColor);
        shader->SetUniformVector4(ShaderParams::PointLightPositionAndRange.data(), pointLightPositionAndRange);

        const Transform *directLightTransform = renderProperties.directLight->GetEntity()->GetComponent<Transform>();
        Vector3 directLightDirection = directLightTransform->GetForward();
        Vector4 directLightColor = renderProperties.directLight->GetColor();
        directLightColor.w = renderProperties.directLight->GetIntesity();

        shader->SetUniformVector4(ShaderParams::DirectLightColor.data(), directLightColor);
        shader->SetUniformVector3(ShaderParams::DirectLightDir.data(), directLightDirection);

        const Transform *spotLightTransform = renderProperties.spotLight->GetEntity()->GetComponent<Transform>();
        Vector4 spotLightDirAndAngle = spotLightTransform->GetForward();
        spotLightDirAndAngle.w = renderProperties.spotLight->GetSpotAngle();
        Vector4 spotLightColor = renderProperties.spotLight->GetColor();
        spotLightColor.w = renderProperties.spotLight->GetIntesity();
        Vector4 spotLightPosAndRange = spotLightTransform->GetPosition();
        spotLightPosAndRange.w = renderProperties.spotLight->GetRange();

        shader->SetUniformVector4(ShaderParams::SpotLightColor.data(), spotLightColor);
        shader->SetUniformVector4(ShaderParams::SpotLightPositionAndRange.data(), spotLightPosAndRange);
        shader->SetUniformVector4(ShaderParams::SpotLightDirAndAngle.data(), spotLightDirAndAngle);
        Print_Errors_OpengGL();
    }
    
    void MeshRenderer::SetShadowLightUniform(const RenderProperties &renderProperties, Material &workingMaterial)
    {
        using namespace SMeshRenderer;
        
        const Shader *shader = workingMaterial.GetShader();
        if(renderProperties.directShadowLight)
        {
            std::vector<Matrix4> lightViewProjectionMatrices = SMeshRenderer::GetDirectShadowMatrices(renderProperties);
            shader->SetUniformMatrices4(ShaderParams::DirectShadowLightViewProj.data(), lightViewProjectionMatrices);
            shader->SetUniformInt(
                                  ShaderParams::CascadesCount.data(),
                                  static_cast<int32_t>(renderProperties.cascadeBorders.size()) + 1);
        }

        if(renderProperties.pointShadowLight)
        {
            std::vector<Matrix4> matrices = GetOmnidirectionalShadowLightMatrices(renderProperties.pointShadowLight);
            const Light* light = renderProperties.pointShadowLight->GetComponent<Light>();
            shader->SetUniformMatrices4(ShaderParams::OmniShadowLightViewProj.data(), matrices);
            shader->SetUniformFloat(ShaderParams::FarClipPlane.data(), light->GetRange());
        }
    }
}
