//#include <Feng/ResourcesManager/ResourceManagerShaders.h>
//
//#include <Feng/App/GuidGenerator.h>
//
//namespace feng
//{
//    ResourceManagerShaders::ResourceManagerShaders(CachedShader *cachedShaders, unsigned cachedShadersCount) :
//        ResourceManager<Shader, CachedShader>(cachedShaders, cachedShadersCount)
//    {
//    }
//
//    Shader * ResourceManagerShaders::Instantiate(CachedShader cachedShader)
//    {
//        GuidGenerator::SetNextResourceId(cachedShader.Id);
//        return new Shader(cachedShader);
//    }
//}
