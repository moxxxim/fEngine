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

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace feng
{
    MeshRenderer::~MeshRenderer()
    {
        DeleteGeomertyBuffers();
        DeleteTextureBuffers();
    }

    void MeshRenderer::SetMaterial(const Material *aMaterial)
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

    void MeshRenderer::SetMesh(const Mesh *aMesh)
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
            SetMaterialUniforms();
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

    void MeshRenderer::SetMaterialUniforms()
    {
        const Shader* shader = material->GetShader();
        const std::map<std::string, int> uniforms = shader->GetUniforms();

        uint32_t textureUnit = 0;

        for(const auto& [name, location] : uniforms)
        {
            int intValue;
            if(material->TryGetInt(name, intValue))
            {
                shader->SetUniformInt(name.c_str(), intValue);
                continue;
            }

            float floatValue;
            if(material->TryGetFloat(name, floatValue))
            {
                shader->SetUniformFloat(name.c_str(), floatValue);
                continue;
            }

            Vector2 vector2Value;
            if(material->TryGetVector2(name, vector2Value))
            {
                shader->SetUniformVector2(name.c_str(), vector2Value);
                continue;
            }

            Vector3 vector3Value;
            if(material->TryGetVector3(name, vector3Value))
            {
                shader->SetUniformVector3(name.c_str(), vector3Value);
                continue;
            }

            Vector4 vector4Value;
            if(material->TryGetVector4(name, vector4Value))
            {
                shader->SetUniformVector4(name.c_str(), vector4Value);
                continue;
            }

            if(const Texture *texture = material->GetTexture(name))
            {
                ActivateTexture(name, *texture, textureUnit);
                ++textureUnit;
                continue;
            }
        }
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
        const std::map<std::string, const Texture*> &textures = material->GetTextures();

        for(const auto& [name, texture] : textures)
        {
            uint32_t tbo = CreateTextureBuffer(*texture);
            textureBuffers[name] = tbo;
        }
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

        AttributesParser attributes { mesh->GetAttributes() };

        GLuint attributeIndex = 0;
        GLsizei offset = 0;
        while(attributes.HasCurrent())
        {
            if(attributes.IsCurrentSet())
            {
                GLint componentsNumber;
                GLenum type;
                GLboolean isNormalized;
                GLsizei size;
                attributes.GetCurrent(componentsNumber, type, isNormalized, size);

                glVertexAttribPointer(
                                    attributeIndex,
                                    componentsNumber,
                                    type,
                                    isNormalized,
                                    static_cast<GLsizei>(mesh->GetVertexStride()),
                                    reinterpret_cast<const GLvoid *>(offset));
                glEnableVertexAttribArray(attributeIndex);
                offset += size;
                ++attributeIndex;
            }

            attributes.Next();
        }

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

    uint32_t MeshRenderer::CreateTextureBuffer(const Texture& texture)
    {
        GLenum target = ToOpenGLValue(texture.GetType());

        GLuint tbo;
        glGenTextures(1, &tbo);
        glBindTexture(target, tbo);

        GLint wrapModeS = static_cast<GLint>(ToOpenGLValue(texture.GetWrapModeS()));
        GLint wrapModeT = static_cast<GLint>(ToOpenGLValue(texture.GetWrapModeT()));
        GLint minFilter = static_cast<GLint>(ToOpenGLValue(texture.GetMinFilter()));
        GLint magFilter = static_cast<GLint>(ToOpenGLValue(texture.GetMagFilter()));

        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapModeS);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapModeT);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

        GenerateTexture(texture);
        if (texture.HasMipMaps())
        {
            glGenerateMipmap(target);
        }

        glBindTexture(target, 0);

        return tbo;
    }

    void MeshRenderer::GenerateTexture(const Texture& texture)
    {
        eTextureType type = texture.GetType();
        if (type == eTextureType::Tex2d)
        {
            GenerateTexture2D(texture);
        }
        else if (type == eTextureType::CubeMap)
        {
            GenerateTextureCube(texture);
        }
    }

    void MeshRenderer::GenerateTexture2D(const Texture& texture)
    {
        GLenum format = GetTextureFormat(texture);
        GLsizei width = static_cast<GLsizei>(texture.GetWidth());
        GLsizei height = static_cast<GLsizei>(texture.GetHeight());
        const GLvoid *data = static_cast<const GLvoid*>(texture.GetData());
        // TODO: m.alekseev Format here is used incorrectly.
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    void MeshRenderer::GenerateTextureCube(const Texture& texture)
    {
//        const GLint format = feng::GetTextureFormat(texture);
//        int faceWidth = texture->GetWidth() / 4;
//        int faceHeight = texture->GetHeight() / 3;
//        for (int i = 0; i < 6; ++i)
//        {
//            const char *data = texture->GetCubeMapFace((feng::CubeMapFace)i);
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, data);
//        }
    }

    void MeshRenderer::SetCameraUniforms(const RenderProperties &renderProperties)
    {
        const Shader *shader = material->GetShader();
        const Entity *camEntity = renderProperties.cam->GetEntity();
        const Transform *camTransform = camEntity->GetComponent<Transform>();
        const Transform *myTransform = GetEntity()->GetComponent<Transform>();

        shader->SetUniformMatrix4(feng::ShaderParams::ModelMatrix.data(), myTransform->GetGlobalMatrix());
        shader->SetUniformMatrix4(feng::ShaderParams::ViewProjMatrix.data(), renderProperties.cam->GetViewProjectionMatrix());
        shader->SetUniformVector3(feng::ShaderParams::CameraPos.data(), camTransform->GetPosition());
        shader->SetUniformVector3(feng::ShaderParams::CameraDir.data(), camTransform->GetForward());
    }

    void MeshRenderer::SetLightUniforms(const RenderProperties &renderProperties)
    {
        const Shader *shader = material->GetShader();
        shader->SetUniformVector4(ShaderParams::AmbientColor.data(), renderProperties.ambientColor);

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
        Vector4 spotLightDirectionAndAngle = spotLightTransform->GetForward();
        spotLightDirectionAndAngle.w = renderProperties.spotLight->GetSpotAngle();
        Vector4 spotLightColor = renderProperties.spotLight->GetColor();
        spotLightColor.w = renderProperties.spotLight->GetIntesity();
        Vector4 spotLightPosAndRange = spotLightTransform->GetPosition();
        spotLightPosAndRange.w = renderProperties.spotLight->GetRange();

        shader->SetUniformVector4(ShaderParams::SpotLightColor.data(), spotLightColor);
        shader->SetUniformVector4(ShaderParams::SpotLightPositionAndRange.data(), spotLightPosAndRange);
        shader->SetUniformVector4(ShaderParams::SpotLightDirAndAngle.data(), spotLightDirectionAndAngle);

//        const char *ambientLightColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::AmbientLightColor);
//        m_shader->SetUniformVector4(ambientLightColorUniform, renderProperties->GetAmbientLightColor());
//
//        int directLightsCount = renderProperties->GetDirectLightsCount();
//        if (directLightsCount > 0)
//        {
//            const char *lightsCountUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::DirectLightCount);
//            m_shader->SetUniformInt(lightsCountUniform, directLightsCount);
//            const char *lightDirectionsUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::DirectLightDirection);
//            const Vector3 *directions = renderProperties->GetDirectLightDirections();
//            m_shader->SetUniformVector3Array(lightDirectionsUniform, directions, directLightsCount);
//            const char *lightColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::DirectLightColor);
//            const Vector4 *colors = renderProperties->GetDirectLightColors();
//            m_shader->SetUniformVector4Array(lightColorUniform, colors, directLightsCount);
//            const char *lightsIntensityUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::DirectLightIntensity);
//            const float *intensities = renderProperties->GetDirectLightIntensities();
//            m_shader->SetUniformFloatArray(lightsIntensityUniform, intensities, directLightsCount);
//        }

//        int pointLightsCount = renderProperties->GetPointLightsCount();
//        if (pointLightsCount > 0)
//        {
//            const char *lightsCountUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightCount);
//            m_shader->SetUniformInt(lightsCountUniform, pointLightsCount);
//            const char *lightPositionsUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightPosition);
//            m_shader->SetUniformVector3Array(lightPositionsUniform, renderProperties->GetPointLightPositions(), pointLightsCount);
//            const char *lightsRangeUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightRange);
//            m_shader->SetUniformFloatArray(lightsRangeUniform, renderProperties->GetPointLightRanges(), pointLightsCount);
//            const char *lightColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightColor);
//            m_shader->SetUniformVector4Array(lightColorUniform, renderProperties->GetPointLightColors(), pointLightsCount);
//            const char *lightsIntensityUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::PointLightIntensity);
//            m_shader->SetUniformFloatArray(lightsIntensityUniform, renderProperties->GetPointLightIntensities(), pointLightsCount);
//        }
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
