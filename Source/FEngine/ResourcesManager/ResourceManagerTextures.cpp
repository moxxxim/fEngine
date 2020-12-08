//#include <FEngine/ResourcesManager/ResourceManagerTextures.h>
//
//#include <FEngine/App/GuidGenerator.h>
//
//feng::ResourceManagerTextures::ResourceManagerTextures(CachedTexture *cachedTextures, unsigned cachedTexturesCount) :
//	ResourceManager<Texture, CachedTexture>(cachedTextures, cachedTexturesCount)
//{
//}
//
//feng::Texture * feng::ResourceManagerTextures::Instantiate(CachedTexture cachedTexture)
//{
//	int width;
//	int height;
//	int bpp;
//	char *textureData = LoadTGA(cachedTexture.Location, &width, &height, &bpp);
//
//	feng::GuidGenerator::SetNextResourceId(cachedTexture.Id);
//	return new Texture(cachedTexture, textureData, width, height, bpp);
//}
