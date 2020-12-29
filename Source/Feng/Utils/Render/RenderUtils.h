#pragma once

#include <Feng/Utils/Render/MeshParams.h>

#include <cstdint>
#include <map>
#include <string>

namespace feng
{
    class Material;
    class Texture;

    void EnableVertexAttributes(eVertexAtributes attributesMask);
    void BindMaterialUniforms(
                            const Material &material,
                            const std::map<std::string, uint32_t>& textureBuffers,
                            uint32_t firstTextureUnit = 0);

    std::map<std::string, uint32_t> CreateTextureBuffers(const Material &material);
    uint32_t CreateTextureBuffer(const Texture& texture);
    void LoadTextureData(const Texture& texture);
}
