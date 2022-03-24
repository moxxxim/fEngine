#include <Feng/ScenesManager/MeshRenderer.h>

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
        }

        Print_Errors_OpengGL();
        
        UpdateInstanceBuffer(instances);
        Print_Errors_OpengGL();

        instancesCount = static_cast<uint32_t>(instances.size());
    }

    bool MeshRenderer::CanDraw() const
    {
        if ((material != nullptr) && (mesh != nullptr))
        {
            return material->HasShader();
        }

        return false;
    }

    void MeshRenderer::Draw(const RenderProperties &renderProperties)
    {
        if (CanDraw())
        {
            StartDraw();
            SetGlobalUniforms(renderProperties);
            Render::BindMaterialUniforms(*material, textureBuffers);
            ExecuteDraw();
            FinishDraw();
        }
    }

    void MeshRenderer::StartDraw()
    {
        material->Apply();
    }

    void MeshRenderer::FinishDraw()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
    }

    void MeshRenderer::SetGlobalUniforms(const RenderProperties &renderProperties)
    {
        if(instancesCount == 0)
        {
            const Shader *shader = material->GetShader();
            const Transform *myTransform = GetEntity()->GetComponent<Transform>();
            shader->SetUniformMatrix4(ShaderParams::ModelMatrix.data(), myTransform->GetGlobalMatrix());
        }

        SetCameraUniforms(renderProperties);
        SetLightUniforms(renderProperties);
        SetFogUniforms(renderProperties);
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
    }

    void MeshRenderer::ActivateTexture(const std::string& name, const Texture &texture, uint32_t unit)
    {
        GLenum target = ToOpenGLValue(texture.GetType());
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(target, textureBuffers[name]);

        const Shader *shader = material->GetShader();
        shader->SetUniformInt(name.c_str(), unit);
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

    void MeshRenderer::SetCameraUniforms(const RenderProperties &renderProperties)
    {
        const Shader *shader = material->GetShader();
        
        shader->SetUniformFloat(ShaderParams::NearClipPlane.data(), renderProperties.cam->GetNearClipPlane());
        shader->SetUniformFloat(ShaderParams::FarClipPlane.data(), renderProperties.cam->GetFarClipPlane());
        shader->SetUniformBuffer(ShaderParams::CamUniformBlock.data(), RenderProperties::CamBufferBinding);
    }

    void MeshRenderer::SetLightUniforms(const RenderProperties &renderProperties)
    {
        const Shader *shader = material->GetShader();
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
    }

    void MeshRenderer::SetFogUniforms(const RenderProperties &renderProperties)
    {
//        const feng::Camera *cam = renderProperties->GetCamera();
//        const feng::Fog& fog = renderProperties->GetFog();
//
//        const char *fogStartUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::FogStart);
//        m_shader->SetUniformFloat(fogStartUniform, fog.GetStart());
//        const char *fogRangeUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::FogRange);
//        m_shader->SetUniformFloat(fogRangeUniform, fog.GetRange());
//        const char *fogDensityUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::FogDensity);
//        m_shader->SetUniformFloat(fogDensityUniform, fog.GetDensity());
//        const char *fogColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::FogColor);
//        m_shader->SetUniformVector3(fogColorUniform, fog.GetColor());
    }
}
