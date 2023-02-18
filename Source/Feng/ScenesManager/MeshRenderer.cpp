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
#include <Feng/Utils/Render/TextureParams.h>
#include <Feng/Utils/Render/RenderUtils.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    namespace SMeshRenderer
    {
        Matrix4 GetShadowCastLightViewMatrix(const Entity* light)
        {
            const Transform *lightTransform = light->GetComponent<Transform>();
            const Matrix4 lightTransformMatrix = lightTransform->GetGlobalMatrix();

            Matrix4 transformInverted;
            std::ignore = lightTransformMatrix.TryInvert(transformInverted);
            return transformInverted;
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
            mesh = aMesh;
            DeleteGeomertyBuffers();
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
            DeleteInstanceBuffer();
            CreateInstanceBuffer();
            Print_Errors_OpengGL();
        }

        UpdateInstanceBuffer(instances);
        Print_Errors_OpengGL();

        instancesCount = static_cast<uint32_t>(instances.size());
    }

    void MeshRenderer::Draw(const RenderProperties &renderProperties, Material *externalMaterial /*= nullptr*/)
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
            SetGlobalUniforms(renderProperties, *workingMaterial);
            Render::BindMaterialUniforms(*workingMaterial, *workingTextures);
            ExecuteDraw();
            FinishDraw();
            
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
        glBindVertexArray(vao);

        ePrimitiveType primitiveType = mesh->GetPrimitiveType();

        // No need to call 'glBindBuffer' because it was bound while VAO was still bound.

        if(ibo != Render::UndefinedBuffer)
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
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        vbo = CreateVertexBuffer();

        if(mesh->HasIndices())
        {
            ibo = CreateIndexBuffer();
        }

        glBindVertexArray(Render::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::UndefinedBuffer);
    }

    void MeshRenderer::DeleteGeomertyBuffers()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &instancesBuffer);

        vao = vbo = ibo = instancesBuffer = Render::UndefinedBuffer;
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
        glBindVertexArray(vao);

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
            glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(Matrix4), instances.data(), GL_STATIC_DRAW);
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
        if(renderProperties.shadowLight)
        {
            Matrix4 lightProjection = Mat4::MakeOrthogonalProjection(20, 20, 1, 200);
            Matrix4 lightViewMatrix = SMeshRenderer::GetShadowCastLightViewMatrix(renderProperties.shadowLight);
            Matrix4 lightViewProjectionMatrix = lightViewMatrix * lightProjection;
            const Shader *shader = workingMaterial.GetShader();
            shader->SetUniformMatrix4(ShaderParams::ShadowLightViewProj.data(), lightViewProjectionMatrix);
        }
    }
}
