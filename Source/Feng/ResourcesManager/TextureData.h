#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace feng
{
    class TextureData final
    {
    public:
        static std::unique_ptr<TextureData> Load(const std::string& path, bool flipOnLoad);
        static std::array<std::unique_ptr<TextureData>, 6> LoadCubemap(
                                                                const std::array<std::string, 6>& paths,
                                                                const std::array<bool, 6>& flipOnLoad);

        TextureData() = default;
        ~TextureData();

        bool IsValid() const;
        const uint8_t* GetData() const;
        int32_t GetWidth() const;
        int32_t GetHeight() const;
        int32_t GetChanels() const;

    private:
        uint8_t *data = nullptr;
        int32_t width;
        int32_t height;
        int32_t chanelsCount;
    };
}

namespace feng
{
    inline bool TextureData::IsValid() const
    {
        return data != nullptr;
    }

    inline const uint8_t* TextureData::GetData() const
    {
        return data;
    }

    inline int32_t TextureData::GetWidth() const
    {
        return width;
    }

    inline int32_t TextureData::GetHeight() const
    {
        return height;
    }

    inline int32_t TextureData::GetChanels() const
    {
        return chanelsCount;
    }
}
