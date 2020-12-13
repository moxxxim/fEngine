#include <Feng/ResourcesManager/Texture.h>

#include <Feng/ResourcesManager/TextureData.h>

namespace feng
{
    Texture::Texture(const TextureData& aData)
        : data { aData }
    { }

    int32_t Texture::GetWidth() const
    {
        return data.GetWidth();
    }

    int32_t Texture::GetHeight() const
    {
        return data.GetHeight();
    }

    int32_t Texture::GetChanels() const
    {
        return data.GetChanels();
    }

    const uint8_t* Texture::GetData() const
    {
        return data.GetData();
    }
}
