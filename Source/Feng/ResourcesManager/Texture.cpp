//#include "Texture.h"
//#include "ResourcesManager.h"
//#include "CachedTexture.h"
//
//feng::Texture::Texture(const CachedTexture& texture, char *data, int width, int height, int bpp) :
//	Object(ObjectType::Resource),
//	m_textureInfo(texture),
//	m_data(data),
//	m_width(width),
//	m_height(height),
//	m_bitsPerPixel(bpp)
//{
//}
//
//feng::Texture::~Texture()
//{
//	delete[] m_data;
//	delete[] m_cubeMap_Pos_X;
//	delete[] m_cubeMap_Neg_X;
//	delete[] m_cubeMap_Pos_Y;
//	delete[] m_cubeMap_Neg_Y;
//	delete[] m_cubeMap_Pos_Z;
//	delete[] m_cubeMap_Neg_Z;
//}
//
//void feng::Texture::Destroy()
//{
//	ResourcesManager *manager = ResourcesManager::GetInstance();
//	manager->DestroyResource(GetId());
//}
//
//const char * feng::Texture::GetCubeMapFace(feng::CubeMapFace face) const
//{
//	int faceWidth = m_width / 4;
//	int faceHeight = m_height / 3;
//	int bytesPerPixel = m_bitsPerPixel / 8;
//
//	switch (face)
//	{
//		case feng::CubeMapFace::XPos:
//			if (!m_cubeMap_Pos_X)
//			{
//				m_cubeMap_Pos_X = CreateCubeMapFace(m_width * bytesPerPixel * faceHeight + 2 * faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Pos_X;
//
//		case feng::CubeMapFace::XNeg:
//			if (!m_cubeMap_Neg_X)
//			{
//				m_cubeMap_Neg_X = CreateCubeMapFace(m_width * bytesPerPixel * faceHeight, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Neg_X;
//
//		case feng::CubeMapFace::YPos:
//			if (!m_cubeMap_Pos_Y)
//			{
//				m_cubeMap_Pos_Y = CreateCubeMapFace(faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Pos_Y;
//
//		case feng::CubeMapFace::YNeg:
//			if (!m_cubeMap_Neg_Y)
//			{
//				m_cubeMap_Neg_Y = CreateCubeMapFace(2 * m_width * bytesPerPixel * faceHeight + faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Neg_Y;
//
//		case feng::CubeMapFace::ZPos:
//			if (!m_cubeMap_Pos_Z)
//			{
//				m_cubeMap_Pos_Z = CreateCubeMapFace(m_width * bytesPerPixel * faceHeight + faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Pos_Z;
//
//		case feng::CubeMapFace::ZNeg:
//			if (!m_cubeMap_Neg_Z)
//			{
//				m_cubeMap_Neg_Z = CreateCubeMapFace(m_width * bytesPerPixel * faceHeight + 3 * faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Neg_Z;
//
//		default:
//			return nullptr;
//	}
//}
//
//char * feng::Texture::CreateCubeMapFace(unsigned start, unsigned width, unsigned height) const
//{
//	unsigned int size = width * height * m_bitsPerPixel / 8;
//	char *face = new char[size];
//
//	unsigned int copySize = width * m_bitsPerPixel / 8;
//	unsigned int padding = 4 * copySize;
//	for (unsigned int i = 0; i < height; ++i)
//	{
//		char *destination = face + i * copySize;
//		char *source = m_data + (start + i * padding);
//		memcpy_s(destination, copySize, source, copySize);
//	}
//
//	return face;
//}
