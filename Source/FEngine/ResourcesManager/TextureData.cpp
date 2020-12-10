#include <FEngine/ResourcesManager/TextureData.h>

#include <Classes/TextureLoader.h>

namespace feng
{
    std::unique_ptr<TextureData> TextureData::Load(const std::string& path, bool flipOnLoad)
    {
        std::unique_ptr<TextureData> texture = std::make_unique<TextureData>();

        stbi_set_flip_vertically_on_load(flipOnLoad);
        stbi_uc *textureData = stbi_load(path.c_str(), &texture->width, &texture->height, &texture->chanelsCount, 0);
        texture->data = static_cast<unsigned char*>(textureData);

        return texture;
    }

    TextureData::~TextureData()
    {
        if(data != nullptr)
        {
            stbi_image_free(data);
        }
    }
}
