#pragma once

#include <FEngine/ResourcesManager/CachedResource.h>
#include <FEngine/ResourcesManager/TextureParameters.h>

namespace feng
{
    class CachedTexture : public CachedResource
    {
    public:
        CachedTexture() = default;
        CachedTexture(const CachedTexture& other);
        CachedTexture(CachedTexture&& other);
        ~CachedTexture();

        CachedTexture& operator = (const CachedTexture& other);
        CachedTexture& operator = (CachedTexture&& other);

        char *Location = nullptr;
        TextureType Type = TextureType::Texture2D;
        TextureWarapMode WrapMode = TextureWarapMode::Repeat;
        TextureMinFilter MinFilter = TextureMinFilter::Nearest;
        TextureMagFilter MagFilter = TextureMagFilter::Nearest;
        bool GenerateMipMaps = false;
    };

    inline CachedTexture::~CachedTexture()
    {
        delete[] Location;
    }
}
