#pragma once

#include <cstdint>
#include <string>

namespace feng
{
    class TextureData final
    {
    public:
        static std::unique_ptr<TextureData> Load(const std::string& path, bool flipOnLoad);

        TextureData() = default;
        ~TextureData();

        bool IsValid() const;
        const unsigned char* GetData() const;
        int32_t GetWidth() const;
        int32_t GetHeight() const;
        int32_t GetChanelsCount() const;

    private:
        unsigned char *data = nullptr;
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

    inline const unsigned char* TextureData::GetData() const
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

    inline int32_t TextureData::GetChanelsCount() const
    {
        return chanelsCount;
    }
}
