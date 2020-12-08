//#include <FEngine/ResourcesManager/CachedMaterial.h>
//#include <utility>
//
//namespace feng
//{
//    CachedMaterial::CachedMaterial(const CachedMaterial& other) :
//        CachedResource(other),
//        ShaderId(other.ShaderId),
//        Textures(other.Textures),
//        Floats(other.Floats),
//        Vec4Values(other.Vec4Values)
//    {
//    }
//
//    CachedMaterial::CachedMaterial(CachedMaterial&& other) :
//        CachedResource(std::move(other)),
//        ShaderId(other.ShaderId),
//        Textures(std::move(other.Textures)),
//        Floats(std::move(other.Floats)),
//        Vec4Values(std::move(other.Vec4Values))
//    {
//    }
//
//    CachedMaterial& CachedMaterial::operator = (const CachedMaterial& other)
//    {
//        if (this != &other)
//        {
//            CachedResource::operator=(other);
//            Textures = other.Textures;
//            Floats = other.Floats;
//            Vec4Values = other.Vec4Values;
//            ShaderId = other.ShaderId;
//        }
//
//        return *this;
//    }
//
//    CachedMaterial& CachedMaterial::operator = (CachedMaterial&& other)
//    {
//        if (this != &other)
//        {
//            CachedResource::operator=(std::move(other));
//            Textures = std::move(other.Textures);
//            Floats = std::move(other.Floats);
//            Vec4Values = std::move(other.Vec4Values);
//            ShaderId = other.ShaderId;
//        }
//
//        return *this;
//    }
//}
