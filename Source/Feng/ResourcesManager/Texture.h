#pragma once

#include <Feng/Utils/Render/TextureParams.h>

#include <array>
#include <cstdint>

namespace feng
{
    class TextureData;

    class Texture final
    {
    public:
        Texture(const TextureData& aData);
        Texture(const std::array<const TextureData*, 6>& aCubemapFaces);

        Texture(const Texture& other) = delete;
        Texture(Texture&& other) = delete;
        Texture& operator = (const Texture& other) = delete;
        Texture& operator = (Texture&& other) = delete;

        int32_t GetWidth() const;
        int32_t GetHeight() const;
        int32_t GetChanels() const;

	    const uint8_t* GetData() const;
        const uint8_t* GetCubemapFaceData(eCubemapFace face) const;
        eTextureType GetType() const;
	    eTextureWrapping GetWrapS() const;
        eTextureWrapping GetWrapT() const;
        eTextureWrapping GetWrapR() const;
	    eTextureMinFilter GetMinFilter() const;
	    eTextureMagFilter GetMagFilter() const;
	    bool HasMipMaps() const;

        void SetWrapping(eTextureWrapping sWrap, eTextureWrapping tWrap);
        void SetWrapping(eTextureWrapping sWrap, eTextureWrapping tWrap, eTextureWrapping rWrap);
        void SetFilters(eTextureMinFilter aMinFilter, eTextureMagFilter aMagFilter);
        void SetUseMipmaps(bool useMipMaps);

    private:
        std::array<const TextureData*, 6> data;

        eTextureType type = eTextureType::Tex2d;
        eTextureWrapping wrapS = eTextureWrapping::Repeat;
        eTextureWrapping wrapT = eTextureWrapping::Repeat;
        eTextureWrapping wrapR = eTextureWrapping::Repeat;
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

    inline eTextureWrapping Texture::GetWrapS() const
    {
        return wrapS;
    }

    inline eTextureWrapping Texture::GetWrapT() const
    {
        return wrapT;
    }

    inline eTextureWrapping Texture::GetWrapR() const
    {
        return wrapR;
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

    inline void Texture::SetWrapping(eTextureWrapping sWrap, eTextureWrapping tWrap)
    {
        wrapS = sWrap;
        wrapT = tWrap;
    }

    inline void Texture::SetWrapping(eTextureWrapping sWrap, eTextureWrapping tWrap, eTextureWrapping rWrap)
    {
        SetWrapping(sWrap, tWrap);
        wrapR = rWrap;
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
