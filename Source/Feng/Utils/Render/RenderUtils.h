#pragma once

#include <Feng/Math/Vector4.h>
#include <Feng/Math/Matrix4.h>
#include <Feng/Render/VertexBuffer.h>
#include <Feng/Utils/Render/MeshParams.h>
#include <Feng/Utils/Render/TextureParams.h>
#include <OpenGL/gl.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Feng
{
    class Camera;
    class Material;
    class Texture;
    enum class eShaderType : uint8_t;
    
    namespace Render
    {
        uint32_t EnableVertexAttributes(eVertexAtributes attributesMask);
        void BindMaterialUniforms(
                                const Material &material,
                                const std::map<std::string, uint32_t>& textureBuffers,
                                uint32_t firstTextureUnit = 0);
        void BindTexture(
                         const Material &material,
                         eTextureType textureType,
                         uint32_t unit,
                         const std::string_view& name,
                         uint32_t textureBuffer);

        std::map<std::string, uint32_t> CreateTextureBuffers(const Material &material);
        uint32_t CreateTextureBuffer(const Texture& texture);
        void LoadTextureData(const Texture& texture);
        GLenum ToOpenGLValue(eShaderType shaderType);
        VertexBuffer CreateQuadBuffer();
        std::array<Vector4, 8> GetFrustumXyzMinMax(const Matrix4& camViewProjInverse);
    }
}
