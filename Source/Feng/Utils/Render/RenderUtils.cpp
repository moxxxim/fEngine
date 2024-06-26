#include <Feng/Utils/Render/RenderUtils.h>

#include <Feng/Core/FengGL.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/ResourcesManager/Texture.h>
#include <Feng/Utils/Render/TextureParams.h>
#include <Feng/Utils/Debug.h>

namespace Feng::Render
{
    namespace SRenderUtils
    {
        void LoadTextureData2d(const Texture& texture)
        {
            GLenum format = GetTextureFormat(texture);
            GLsizei width = static_cast<GLsizei>(texture.GetWidth());
            GLsizei height = static_cast<GLsizei>(texture.GetHeight());
            const GLvoid *data = static_cast<const GLvoid*>(texture.GetData());
            // TODO: m.alekseev Format here is used incorrectly.
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }

        void LoadTextureDataCube(const Texture& texture)
        {
            const GLint format = GetTextureFormat(texture);
            GLsizei faceWidth = static_cast<GLsizei>(texture.GetWidth());
            GLsizei faceHeight = static_cast<GLsizei>(texture.GetHeight());
            for (int32_t i = 0; i < 6; ++i)
            {
                const uint8_t *data = texture.GetCubemapFaceData(static_cast<eCubemapFace>(i));
                const GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
                // TODO: m.alekseev Format here is used incorrectly.
                glTexImage2D(target, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, data);
            }
        }
    }

    uint32_t EnableVertexAttributes(eVertexAtributes enabled, eVertexAtributes all)
    {
        uint32_t stride = AttributesParser::GetStride(all);
        AttributesParser attributes { enabled };

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
                                    static_cast<GLsizei>(stride),
                                    reinterpret_cast<const GLvoid *>(static_cast<uintptr_t>(offset)));
                Print_Errors_OpengGL();
                
                glEnableVertexAttribArray(attributeIndex);
                Print_Errors_OpengGL();
                offset += size;
                ++attributeIndex;
            }

            attributes.Next();
        }
        
        return attributeIndex;
    }

    void ResolveBindings(Shader& shader,
                         const ShaderBindings& bindings,
                         const std::map<std::string, uint32_t>& textureBuffers,
                         uint32_t firstTextureUnit)
    {
        const std::map<std::string, int> uniforms = shader.GetUniforms();

        uint32_t textureUnit = firstTextureUnit;

        for(const auto& [name, location] : uniforms)
        {
            int intValue;
            if(bindings.TryGetInt(name, intValue))
            {
                shader.SetUniformInt(name.c_str(), intValue);
                Print_Errors_OpengGL();
                continue;
            }

            float floatValue;
            if(bindings.TryGetFloat(name, floatValue))
            {
                shader.SetUniformFloat(name.c_str(), floatValue);
                Print_Errors_OpengGL();
                continue;
            }

            Vector2 vector2Value;
            if(bindings.TryGetVector2(name, vector2Value))
            {
                shader.SetUniformVector2(name.c_str(), vector2Value);
                Print_Errors_OpengGL();
                continue;
            }

            Vector3 vector3Value;
            if(bindings.TryGetVector3(name, vector3Value))
            {
                shader.SetUniformVector3(name.c_str(), vector3Value);
                Print_Errors_OpengGL();
                continue;
            }

            Vector4 vector4Value;
            if(bindings.TryGetVector4(name, vector4Value))
            {
                shader.SetUniformVector4(name.c_str(), vector4Value);
                Print_Errors_OpengGL();
                continue;
            }
            
            std::vector<float> arrayFloats;
            if(bindings.TryGetArrayFloats(name, arrayFloats))
            {
                shader.SetUniformFloats(name.c_str(), arrayFloats);
                Print_Errors_OpengGL();
                continue;
            }
            
            std::vector<Matrix4> arrayMatrices;
            if(bindings.TryGetArrayMatrices4(name, arrayMatrices))
            {
                shader.SetUniformMatrices4(name.c_str(), arrayMatrices);
                Print_Errors_OpengGL();
                continue;
            }
            
            uint32_t buffer;
            if(bindings.TryGetBuffer(name, buffer))
            {
                shader.SetUniformBuffer(name.c_str(), buffer);
                Print_Errors_OpengGL();
                continue;
            }

            if(const Texture *texture = bindings.GetTexture(name))
            {
                if(auto it = textureBuffers.find(name); it != textureBuffers.end())
                {
                    BindTexture(shader, texture->GetType(), textureUnit, name, it->second);
                    Print_Errors_OpengGL();
                    ++textureUnit;
                }
                else
                {
                    Debug::LogWarning("No buffer for texture found: " + name);
                }
            }
        }
    }
    
    void BindTexture(const Shader &shader, eTextureType textureType, uint32_t unit, const std::string_view& name, uint32_t textureBuffer)
    {
        GLenum target = ToOpenGLValue(textureType);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(target, textureBuffer);
        Print_Errors_OpengGL();
        
        shader.SetUniformInt(name.data(), unit);
    }
    
    std::map<std::string, uint32_t> CreateTextureBuffers(const Material &material)
    {
        std::map<std::string, uint32_t> buffers;

        const std::map<std::string, const Texture*> &textures = material.Bindings().GetTextures();

        for(const auto& [name, texture] : textures)
        {
            uint32_t tbo = CreateTextureBuffer(*texture);
            buffers[name] = tbo;
        }

        return buffers;
    }

    uint32_t CreateTextureBuffer(const Texture& texture)
    {
        eTextureType textureType = texture.GetType();
        GLenum target = ToOpenGLValue(textureType);

        GLuint tbo;
        glGenTextures(1, &tbo);
        glBindTexture(target, tbo);

        GLint wrapModeS = static_cast<GLint>(ToOpenGLValue(texture.GetWrapS()));
        GLint wrapModeT = static_cast<GLint>(ToOpenGLValue(texture.GetWrapT()));

        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapModeS);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapModeT);
        if(textureType == eTextureType::Cubemap)
        {
            GLint wrapModeR = static_cast<GLint>(ToOpenGLValue(texture.GetWrapR()));
            glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapModeR);
        }

        GLint minFilter = static_cast<GLint>(ToOpenGLValue(texture.GetMinFilter()));
        GLint magFilter = static_cast<GLint>(ToOpenGLValue(texture.GetMagFilter()));
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

        LoadTextureData(texture);
        if (texture.HasMipMaps())
        {
            glGenerateMipmap(target);
        }

        glBindTexture(target, 0);

        return tbo;
    }

    void LoadTextureData(const Texture& texture)
    {
        eTextureType type = texture.GetType();
        if (type == eTextureType::Tex2d)
        {
            SRenderUtils::LoadTextureData2d(texture);
        }
        else if (type == eTextureType::Cubemap)
        {
            SRenderUtils::LoadTextureDataCube(texture);
        }
    }

    GLenum ToOpenGLValue(eShaderType shaderType)
    {
        switch (shaderType)
        {
            case eShaderType::Vertex:
                return GL_VERTEX_SHADER;

            case eShaderType::Geometry:
                return GL_GEOMETRY_SHADER;

            case eShaderType::Fragment:
                return GL_FRAGMENT_SHADER;
        }

        Debug::LogError("Invalid shader type specified.");
        return 0;
    }
    
    VertexBuffer CreateQuadBuffer()
    {
        std::array<float, 6 * (3 + 2)> quadVertices
        {
            -1.0, -1.0, 0.0,    0.0, 0.0,
            -1.0,  1.0, 0.0,    0.0, 1.0,
             1.0, -1.0, 0.0,    1.0, 0.0,

             1.0, -1.0, 0.0,    1.0, 0.0,
            -1.0,  1.0, 0.0,    0.0, 1.0,
             1.0,  1.0, 0.0,    1.0, 1.0
        };

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quadVertices.size(), quadVertices.data(), GL_STATIC_DRAW);

        eVertexAtributes attributes = static_cast<eVertexAtributes>(eVertexAtributes::Position | eVertexAtributes::Uv0);
        std::ignore = Render::EnableVertexAttributes(attributes, attributes);

        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
        glBindVertexArray(Render::UndefinedBuffer);
        
        VertexBuffer buffer;
        buffer.vao = vao;
        buffer.vbo = vbo;
        
        return buffer;
    }
    
    std::array<Vector4, 8> GetFrustumXyzMinMax(const Matrix4& camViewProjInverse)
    {
        std::array<Vector4, 8> corners;

        int32_t i = 0;
        for(int x = 0; x < 2; ++x)
        {
            for(int y = 0; y < 2; ++y)
            {
                for(int z = 0; z < 2; ++z)
                {
                    Vector4 cornerNdc {
                        2.f * x - 1.f,
                        2.f * y - 1.f,
                        2.f * z - 1.f,
                        1.f};
                    Vector4 cornerWorld = cornerNdc * camViewProjInverse;
                    corners[i] = cornerWorld / cornerWorld.coord.w;
                    ++i;
                }
            }
        }

        return corners;
    }
}
