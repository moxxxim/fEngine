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

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace feng
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
            if(material != nullptr)
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
            if(mesh != nullptr)
            {
                CreateGeometryBuffers();
            }
        }
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
            BindMaterialUniforms(*material, textureBuffers);
            ExecuteDraw();
            FinishDraw();
        }
    }

    void MeshRenderer::StartDraw()
    {
        const Shader *shader = material->GetShader();
        shader->StartUse();
    }

    void MeshRenderer::FinishDraw()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshRenderer::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, MeshRenderer::UndefinedBuffer);

        const Shader *shader = material->GetShader();
        shader->StopUse();
    }

    void MeshRenderer::SetGlobalUniforms(const RenderProperties &renderProperties)
    {
        SetCameraUniforms(renderProperties);
        SetLightUniforms(renderProperties);
        SetFogUniforms(renderProperties);
    }

    void MeshRenderer::ExecuteDraw()
    {
        glBindVertexArray(vao);

        ePrimitiveType primitiveType = mesh->GetPrimitiveType();

        if(ibo != MeshRenderer::UndefinedBuffer)
        {
            const std::vector<uint32_t>& indices = mesh->GetIndices();

            // No need to call 'glBindBuffer' because it was bound while VAO was still bound.
            glDrawElements(ToOpenGLValue(primitiveType), indices.size(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            int32_t verticesCount = mesh->GetVerticesCount();
            glDrawArrays(ToOpenGLValue(primitiveType), 0, verticesCount);
        }

        glBindVertexArray(UndefinedBuffer);
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

        glBindVertexArray(MeshRenderer::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, MeshRenderer::UndefinedBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshRenderer::UndefinedBuffer);
    }

    void MeshRenderer::DeleteGeomertyBuffers()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &vao);

        vao = vbo = ibo = MeshRenderer::UndefinedBuffer;
    }

    void MeshRenderer::CreateTexturesBuffers()
    {
        textureBuffers = CreateTextureBuffers(*material);
    }

    void MeshRenderer::DeleteTextureBuffers()
    {
        for(const auto& [name, tbo] : textureBuffers)
        {
            glDeleteTextures(1, &tbo);
        }
    }

    uint32_t MeshRenderer::CreateVertexBuffer()
    {
        uint32_t bufferObject;
        glGenBuffers(1, &bufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
        glBufferData(GL_ARRAY_BUFFER, mesh->GetDataSize(), mesh->GetData(), GL_STATIC_DRAW);

        EnableVertexAttributes(mesh->GetAttributes());

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
        const Entity *camEntity = renderProperties.cam->GetEntity();
        const Transform *camTransform = camEntity->GetComponent<Transform>();
        const Transform *myTransform = GetEntity()->GetComponent<Transform>();
        const Camera *cam = camEntity->GetComponent<Camera>();

        shader->SetUniformMatrix4(feng::ShaderParams::ModelMatrix.data(), myTransform->GetGlobalMatrix());
        shader->SetUniformMatrix4(feng::ShaderParams::ViewProjMatrix.data(), renderProperties.cam->GetViewProjectionMatrix());
        shader->SetUniformVector3(feng::ShaderParams::CameraPos.data(), camTransform->GetPosition());
        shader->SetUniformVector3(feng::ShaderParams::CameraDir.data(), camTransform->GetForward());
        shader->SetUniformMatrix3(feng::ShaderParams::CameraRotation.data(), camTransform->GetRotation());
        shader->SetUniformMatrix4(feng::ShaderParams::ProjMatrix.data(), cam->GetProjectionMatrix());
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
