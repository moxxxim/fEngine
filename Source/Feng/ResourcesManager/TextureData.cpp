#include <Feng/ResourcesManager/TextureData.h>

#include <Feng/ResourcesManager/TextureLoader.h>
#include <Feng/Utils/Render/TextureParams.h>

#include <map>

namespace feng
{
    std::array<std::unique_ptr<TextureData>, 6> TextureData::LoadCubemap(
                                                        const std::array<std::string, 6>& paths,
                                                        const std::array<bool, 6>& flipOnLoad)
    {
        std::array<std::unique_ptr<TextureData>, 6> faces;

        for(size_t i = 0; i < paths.size(); ++i)
        {
            const std::string& facePath = paths[i];
            faces[i] = TextureData::Load(facePath, flipOnLoad[i]);
        }

        return faces;
    }

    std::unique_ptr<TextureData> TextureData::Load(const std::string& path, bool flipOnLoad)
    {
        std::unique_ptr<TextureData> texture = std::make_unique<TextureData>();

        stbi_set_flip_vertically_on_load(flipOnLoad);
        stbi_uc *textureData = stbi_load(path.c_str(), &texture->width, &texture->height, &texture->chanelsCount, 0);
        texture->data = static_cast<uint8_t*>(textureData);

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
