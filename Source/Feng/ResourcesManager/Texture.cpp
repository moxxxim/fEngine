#include <Feng/ResourcesManager/Texture.h>

#include <Feng/ResourcesManager/TextureData.h>

namespace feng
{
    Texture::Texture(const TextureData& aData)
        : type { eTextureType::Tex2d }
    {
        data[0] = &aData;
    }

    Texture::Texture(const std::array<const TextureData*, 6>& aCubemapFaces)
        : type { eTextureType::Cubemap }
    {
        for(uint8_t i = 0; i < 6; ++i)
        {
            data[i] = aCubemapFaces[i];
        }
    }

    int32_t Texture::GetWidth() const
    {
        return data.front()->GetWidth();
    }

    int32_t Texture::GetHeight() const
    {
        return data.front()->GetHeight();
    }

    int32_t Texture::GetChanels() const
    {
        return data.front()->GetChanels();
    }

    const uint8_t* Texture::GetData() const
    {
        return data.front()->GetData();
    }

    const uint8_t* Texture::GetCubemapFaceData(eCubemapFace face) const
    {
        return data[static_cast<uint8_t>(face)]->GetData();
    }
}
