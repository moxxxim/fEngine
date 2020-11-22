#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CachedTexture.h"
#include "../Utils/Vector.h"

namespace fengine
{	

class ResourceManagerTextures final: public ResourceManager<Texture, CachedTexture>
{

public:

	ResourceManagerTextures(CachedTexture *cachedTextures, unsigned cachedTexturesCount);

protected:

	Texture * Instantiate(CachedTexture cachedTexture) override;

};

}