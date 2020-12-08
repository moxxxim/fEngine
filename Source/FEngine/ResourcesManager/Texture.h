//#pragma once
//
//#include <FEngine/App/Object.h>
//
//#include <FEngine/ResourcesManager/CachedTexture.h>
//
//namespace feng
//{
//    class Texture final : public Object
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
//        void Destroy() override;
//
//        int GetWidth() const;
//        int GetHeight() const;
//        int GetBpp() const;
//	    const char * GetData() const;
//        TextureType GetType() const;
//	    TextureWarapMode GetWrapMode() const;
//	    TextureMinFilter GetMinFilter() const;
//	    TextureMagFilter GetMagFilter() const;
//	    bool HasMipMaps() const;
//        const char * GetCubeMapFace(CubeMapFace face) const;
//
//    private:
//        char * CreateCubeMapFace(unsigned start, unsigned width, unsigned height) const;
//
//        CachedTexture m_textureInfo;
//        char *m_data = nullptr;
//        int m_width = 0;
//        int m_height = 0;
//        int m_bitsPerPixel = 0;
//
//        mutable char *m_cubeMap_Pos_X = nullptr;
//        mutable char *m_cubeMap_Neg_X = nullptr;
//        mutable char *m_cubeMap_Pos_Y = nullptr;
//        mutable char *m_cubeMap_Neg_Y = nullptr;
//        mutable char *m_cubeMap_Pos_Z = nullptr;
//        mutable char *m_cubeMap_Neg_Z = nullptr;
//    };
//
//    inline int Texture::GetWidth() const
//    {
//        return m_width;
//    }
//
//    inline int Texture::GetHeight() const
//    {
//        return m_height;
//    }
//
//    inline int Texture::GetBpp() const
//    {
//        return m_bitsPerPixel;
//    }
//
//    inline const char * Texture::GetData() const
//    {
//        return m_data;
//    }
//
//    inline TextureType Texture::GetType() const
//    {
//        return m_textureInfo.Type;
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
