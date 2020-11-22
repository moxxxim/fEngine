//#include <FEngine/ResourcesManager/ResourceManagerTextures.h>
//
//#include <FEngine/App/GuidGenerator.h>
//
//fengine::ResourceManagerTextures::ResourceManagerTextures(CachedTexture *cachedTextures, unsigned cachedTexturesCount) :
//	ResourceManager<Texture, CachedTexture>(cachedTextures, cachedTexturesCount)
//{
//}
//
//fengine::Texture * fengine::ResourceManagerTextures::Instantiate(CachedTexture cachedTexture)
//{
//	int width;
//	int height;
//	int bpp;
//	char *textureData = LoadTGA(cachedTexture.Location, &width, &height, &bpp);
//
//	fengine::GuidGenerator::SetNextResourceId(cachedTexture.Id);
//	return new Texture(cachedTexture, textureData, width, height, bpp);
//}
