//#include "MeshLoaderNFG.h"
//
//#include <cstdio>
//#include "Mesh.h"
//#include "../Utils/Debug.h"
//
//const char *const fengine::MeshLoaderNFG::k_verticesCountTag = "NrVertices:";
//const char *const fengine::MeshLoaderNFG::k_indicesCountTag = "NrIndices:";
//const char *const fengine::MeshLoaderNFG::k_positionTag = "pos:";
//const char *const fengine::MeshLoaderNFG::k_normTag = "norm:";
//const char *const fengine::MeshLoaderNFG::k_binormTag = "binorm:";
//const char *const fengine::MeshLoaderNFG::k_tangentTag = "tgt:";
//const char *const fengine::MeshLoaderNFG::k_uvTag = "uv:";
//
//fengine::MeshLoaderNFG::MeshLoaderNFG(const char *path) :
//	m_path(path)
//{
//}
//
//fengine::Mesh * fengine::MeshLoaderNFG::Load()
//{
//	FILE *file = fopen(m_path, "r");
//
//	if (file != nullptr)
//	{
//		unsigned verticesCount;
//		Vertex *vertices = nullptr;
//		ReadVertices(file, vertices, verticesCount);
//
//		unsigned indicesCount;
//		unsigned *indices = nullptr;
//		ReadIndices(file, indices, indicesCount);
//
//		fclose(file);
//
//		Mesh *model = new Mesh(vertices, indices, verticesCount, indicesCount);
//		return model;
//	}
//
//	return nullptr;
//}
//
//void fengine::MeshLoaderNFG::ReadVertices(FILE *file, Vertex *&vertices, unsigned& verticesCount)
//{
//	verticesCount = ReadVerticesCount(file);
//	vertices = new Vertex[verticesCount];
//
//	const char *vertexIndexFormat = "%*u. ";
//	for (unsigned int i = 0; i < verticesCount; ++i)
//	{
//		fscanf_s(file, vertexIndexFormat);
//		ReadVertex(file, vertices[i]);
//	}
//}
//
//unsigned int fengine::MeshLoaderNFG::ReadVerticesCount(FILE *file)
//{
//	unsigned int verticesCount;
//	const char *const format = "%u";
//	const char *whitespace = " ";
//	fscanf_s(file, k_verticesCountTag);
//	fscanf_s(file, whitespace);
//	fscanf_s(file, format, &verticesCount);
//
//	return verticesCount;
//}
//
//void fengine::MeshLoaderNFG::ReadVertex(FILE *file, Vertex& vertex)
//{
//	ReadTaggedVector3(file, k_positionTag, vertex.Position);
//	ReadTaggedVector3(file, k_normTag, vertex.Norm);
//	ReadTaggedVector3(file, k_binormTag, vertex.Binorm);
//	ReadTaggedVector3(file, k_tangentTag, vertex.Tangent);
//	ReadTaggedVector2(file, k_uvTag, vertex.UV1);
//}
//
//void fengine::MeshLoaderNFG::ReadTaggedVector3(FILE *file, const char *tag, Vector3& vec)
//{
//	const char *delimiter = "; ";
//	const char *whitespace = " ";
//	fscanf_s(file, tag);
//	fscanf_s(file, whitespace);
//	ReadVector3(file, vec);
//	fscanf_s(file, delimiter);
//}
//
//void fengine::MeshLoaderNFG::ReadVector3(FILE *file, Vector3& vec)
//{
//	const char *const format = "[%f, %f, %f]";
//	fscanf_s(file, format, &vec.x, &vec.y, &vec.z);
//}
//
//void fengine::MeshLoaderNFG::ReadTaggedVector2(FILE *file, const char *tag, Vector2& vec)
//{
//	const char *delimiter = "; ";
//	const char *whitespace = " ";
//	fscanf_s(file, tag);
//	fscanf_s(file, whitespace);
//	ReadVector2(file, vec);
//	fscanf_s(file, delimiter);
//}
//
//void fengine::MeshLoaderNFG::ReadVector2(FILE *file, Vector2& vec)
//{
//	const char *const format = "[%f, %f]";
//	fscanf_s(file, format, &vec.x, &vec.y);
//}
//
//void fengine::MeshLoaderNFG::ReadIndices(FILE *file, unsigned *&indices, unsigned& indicesCount)
//{
//	indicesCount = ReadIndicesCount(file);
//	indices = new unsigned[indicesCount];
//
//	for (unsigned int i = 0; i < indicesCount; i += 3)
//	{
//		ReadIndicesTriangle(file, indices + i);
//	}
//}
//
//unsigned int fengine::MeshLoaderNFG::ReadIndicesCount(FILE *file)
//{
//	unsigned int indicesCount;
//	const char *const format = "%u";
//	const char *whitespace = " ";
//	fscanf_s(file, k_indicesCountTag);
//	fscanf_s(file, whitespace);
//	fscanf_s(file, format, &indicesCount);
//
//	return indicesCount;
//}
//
//void fengine::MeshLoaderNFG::ReadIndicesTriangle(FILE *file, unsigned int *triangle)
//{
//	const char *const triangleIndexFormat = "%*u. ";
//	const char *const triangleIndicesFormat = "%u, %u, %u, ";
//	fscanf_s(file, triangleIndexFormat);
//
//	unsigned int i1;
//	unsigned int i2;
//	unsigned int i3;
//
//	fscanf_s(file, triangleIndicesFormat, &i1, &i2, &i3);
//	triangle[0] = i1;
//	triangle[1] = i2;
//	triangle[2] = i3;
//}
