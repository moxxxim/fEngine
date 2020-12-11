//#pragma once
//
//#include <Feng/ResourcesManager/CachedTexture.h>
//
//#include <cstdint>
//
//namespace feng
//{
//    class Texture final
//    {
//    public:
//        Texture(const CachedTexture& texture, char *data, int width, int height, int bpp);
//        ~Texture();
//
//        Texture(const Texture& other) = delete;
//        Texture(Texture&& other) = delete;
//        Texture& operator = (const Texture& other) = delete;
//        Texture& operator = (Texture&& other) = delete;
//
//        int32_t GetWidth() const;
//        int32_t GetHeight() const;
//        int32_t GetBpp() const;
//
//	    const char * GetData() const;
//        TextureType GetType() const;
//	    TextureWarapMode GetWrapMode() const;
//	    TextureMinFilter GetMinFilter() const;
//	    TextureMagFilter GetMagFilter() const;
//	    bool HasMipMaps() const;
//
//    private:
//        CachedTexture textureInfo;
//        char *data = nullptr;
//        int32_t width = 0;
//        int32_t height = 0;
//        int32_t bitsPerPixel = 0;
//    };
//}
//
//namespace feng
//{
//    inline int32_t Texture::GetWidth() const
//    {
//        return width;
//    }
//
//    inline int32_t Texture::GetHeight() const
//    {
//        return height;
//    }
//
//    inline int32_t Texture::GetBpp() const
//    {
//        return bitsPerPixel;
//    }
//
//    inline const char * Texture::GetData() const
//    {
//        return data;
//    }
//
//    inline TextureType Texture::GetType() const
//    {
//        return textureInfo.Type;
//    }
//
//    inline TextureWarapMode Texture::GetWrapMode() const
//    {
//        return m_textureInfo.WrapMode;
//    }
//
//    inline TextureMinFilter Texture::GetMinFilter() const
//    {
//        return m_textureInfo.MinFilter;
//    }
//
//    inline TextureMagFilter Texture::GetMagFilter() const
//    {
//        return m_textureInfo.MagFilter;
//    }
//
//    inline bool Texture::HasMipMaps() const
//    {
//        return m_textureInfo.GenerateMipMaps;
//    }
//}
