#pragma once

#include <Feng/Utils/Render/TextureParams.h>

#include <cstdint>

namespace feng
{
    class TextureData;

    class Texture final
    {
    public:
        Texture(const TextureData& aData);

        Texture(const Texture& other) = delete;
        Texture(Texture&& other) = delete;
        Texture& operator = (const Texture& other) = delete;
        Texture& operator = (Texture&& other) = delete;

        int32_t GetWidth() const;
        int32_t GetHeight() const;
        int32_t GetChanels() const;

	    const uint8_t* GetData() const;
        eTextureType GetType() const;
	    eTextureWrapping GetWrapModeS() const;
        eTextureWrapping GetWrapModeT() const;
	    eTextureMinFilter GetMinFilter() const;
	    eTextureMagFilter GetMagFilter() const;
	    bool HasMipMaps() const;

        void SetType(eTextureType aType);
        void SetWrapping(eTextureWrapping sWrap, eTextureWrapping tWrap);
        void SetFilters(eTextureMinFilter aMinFilter, eTextureMagFilter aMagFilter);
        void SetUseMipmaps(bool useMipMaps);

    private:
        const TextureData& data;

        eTextureType type = eTextureType::Tex2d;
        eTextureWrapping wrapS = eTextureWrapping::Repeat;
        eTextureWrapping wrapT = eTextureWrapping::Repeat;
        eTextureMinFilter minFilter = eTextureMinFilter::Nearest;
        eTextureMagFilter magFilter = eTextureMagFilter::Nearest;
        bool generateMipMaps = false;
    };
}

namespace feng
{
    inline eTextureType Texture::GetType() const
    {
        return type;
    }

    inline eTextureWrapping Texture::GetWrapModeS() const
    {
        return wrapS;
    }

    inline eTextureWrapping Texture::GetWrapModeT() const
    {
        return wrapT;
    }

    inline eTextureMinFilter Texture::GetMinFilter() const
    {
        return minFilter;
    }

    inline eTextureMagFilter Texture::GetMagFilter() const
    {
        return magFilter;
    }

    inline bool Texture::HasMipMaps() const
    {
        return generateMipMaps;
    }

    inline void Texture::SetType(eTextureType aType)
    {
        type = aType;
    }

    inline void Texture::SetWrapping(eTextureWrapping sWrap, eTextureWrapping tWrap)
    {
        wrapS = sWrap;
        wrapT = tWrap;
    }

    inline void Texture::SetFilters(eTextureMinFilter aMinFilter, eTextureMagFilter aMagFilter)
    {
        minFilter = aMinFilter;
        magFilter = aMagFilter;
    }

    inline void Texture::SetUseMipmaps(bool useMipMaps)
    {
        generateMipMaps = useMipMaps;
    }
}