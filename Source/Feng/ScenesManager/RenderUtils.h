//#pragma once
//
//#include "../../../Utilities/utilities.h"
//#include "../ResourcesManager/Texture.h"
//
//namespace feng
//{
//
//inline GLint GetTextureFormat(const Texture *texture) 
//{
//	return texture->GetBpp() == 24 ? GL_RGB : GL_RGBA;
//}
//
//inline GLint GetTextureWrapMode(const Texture *texture)
//{
//	return texture->GetWrapMode() == TextureWarapMode::Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
//}
//
//inline GLint GetTextureMinFilter(const Texture *texture) 
//{
//	switch (texture->GetMinFilter())
//	{
//		case TextureMinFilter::Nearest:
//			return GL_NEAREST;
//
//		case TextureMinFilter::Linear:
//			return GL_LINEAR;
//
//		case TextureMinFilter::NearestMipNearest:
//			return GL_NEAREST_MIPMAP_NEAREST;
//
//		case TextureMinFilter::NearestMipLinear:
//			return GL_NEAREST_MIPMAP_LINEAR;
//
//		case TextureMinFilter::LinearMipNearest:
//			return GL_LINEAR_MIPMAP_NEAREST;
//
//		case TextureMinFilter::LinearMipLinear:
//			return GL_LINEAR_MIPMAP_LINEAR;
//
//		default:
//			return GL_NEAREST;
//	}
//}
//
//inline GLint GetTextureMagFilter(const Texture *texture)
//{
//	switch (texture->GetMagFilter())
//	{
//		case TextureMagFilter::Nearest:
//			return GL_NEAREST;
//
//		case TextureMagFilter::Linear:
//			return GL_LINEAR;
//
//		default:
//			return GL_NEAREST;
//	}
//}
//
//}
