#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace Feng
{
    class Material;
    class Shader;
    class Texture;

    class PostEffectRenderer final
    {
    public:
        PostEffectRenderer(Material *aMaterial, const std::map<std::string, uint32_t> &aFrameBufferTextures);
        void Draw();

    private:
        void SetupVertexAttribute();
        void SetupBufferedTextures();
        void SetupTexturesBuffers();
        void ClearTexturesBuffers();

        Material *material = nullptr;
        const std::map<std::string, uint32_t> &frameBufferTextures;
        uint32_t colorBuffer = 0;
        uint32_t depthBuffer = 0;
        std::map<std::string, uint32_t> materialTextures;
    };
}
