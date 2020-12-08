#pragma once

namespace feng
{

enum class TextureWarapMode : int
{
	Repeat = 0,
	Clamp = 1
};

enum class TextureType : int
{
	Texture2D = 0,
	Cube = 1,
};

enum class TextureMinFilter : int
{
	Nearest = 0,
	Linear = 1,
	NearestMipNearest = 2,
	LinearMipNearest = 3,
	NearestMipLinear = 4,
	LinearMipLinear = 5
};

enum class TextureMagFilter : int
{
	Nearest = 0,
	Linear = 1
};

enum class CubeMapFace : int
{
	XPos = 0,
	XNeg = 1,
	YPos = 2,
	YNeg = 3,
	ZPos = 4,
	ZNeg = 5
};


}
