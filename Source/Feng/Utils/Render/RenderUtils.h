#pragma once

#include <Feng/Utils/Render/MeshParams.h>
#include <OpenGL/gl.h>

#include <cstdint>
#include <map>
#include <string>

namespace Feng
{
    class Material;
    class Texture;
    enum class eShaderType : uint8_t;
    
    namespace Render
    {
        void EnableVertexAttributes(eVertexAtributes attributesMask);
        void BindMaterialUniforms(
                                const Material &material,
                                const std::map<std::string, uint32_t>& textureBuffers,
                                uint32_t firstTextureUnit = 0);

        std::map<std::string, uint32_t> CreateTextureBuffers(const Material &material);
        uint32_t CreateTextureBuffer(const Texture& texture);
        void LoadTextureData(const Texture& texture);
        GLenum ToOpenGLValue(eShaderType shaderType);
    }
}
