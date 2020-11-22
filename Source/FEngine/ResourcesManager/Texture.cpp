//#include "Texture.h"
//#include "ResourcesManager.h"
//#include "CachedTexture.h"
//
//fengine::Texture::Texture(const CachedTexture& texture, char *data, int width, int height, int bpp) :
//	Object(ObjectType::Resource),
//	m_textureInfo(texture),
//	m_data(data),
//	m_width(width),
//	m_height(height),
//	m_bitsPerPixel(bpp)
//{
//}
//
//fengine::Texture::~Texture()
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
//void fengine::Texture::Destroy()
//{
//	ResourcesManager *manager = ResourcesManager::GetInstance();
//	manager->DestroyResource(GetId());
//}
//
//const char * fengine::Texture::GetCubeMapFace(fengine::CubeMapFace face) const
//{
//	int faceWidth = m_width / 4;
//	int faceHeight = m_height / 3;
//	int bytesPerPixel = m_bitsPerPixel / 8;
//
//	switch (face)
//	{
//		case fengine::CubeMapFace::XPos:
//			if (!m_cubeMap_Pos_X)
//			{
//				m_cubeMap_Pos_X = CreateCubeMapFace(m_width * bytesPerPixel * faceHeight + 2 * faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Pos_X;
//
//		case fengine::CubeMapFace::XNeg:
//			if (!m_cubeMap_Neg_X)
//			{
//				m_cubeMap_Neg_X = CreateCubeMapFace(m_width * bytesPerPixel * faceHeight, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Neg_X;
//
//		case fengine::CubeMapFace::YPos:
//			if (!m_cubeMap_Pos_Y)
//			{
//				m_cubeMap_Pos_Y = CreateCubeMapFace(faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Pos_Y;
//
//		case fengine::CubeMapFace::YNeg:
//			if (!m_cubeMap_Neg_Y)
//			{
//				m_cubeMap_Neg_Y = CreateCubeMapFace(2 * m_width * bytesPerPixel * faceHeight + faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Neg_Y;
//
//		case fengine::CubeMapFace::ZPos:
//			if (!m_cubeMap_Pos_Z)
//			{
//				m_cubeMap_Pos_Z = CreateCubeMapFace(m_width * bytesPerPixel * faceHeight + faceWidth * bytesPerPixel, faceWidth, faceHeight);
//			}
//
//			return m_cubeMap_Pos_Z;
//
//		case fengine::CubeMapFace::ZNeg:
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
//char * fengine::Texture::CreateCubeMapFace(unsigned start, unsigned width, unsigned height) const
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
