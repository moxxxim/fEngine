//#pragma once
//
//#include "../Utilities/utilities.h"
//#include "Vertex.h"
//
//namespace fengine
//{
//
//class Mesh;
//
//class MeshLoaderNFG final
//{
//
//public:
//
//	MeshLoaderNFG(const char *path);
//	Mesh * Load();
//
//private:
//
//	void ReadVertices(FILE *file, Vertex *&vertices, unsigned& verticesCount);
//	unsigned int ReadVerticesCount(FILE *file);
//	void ReadVertex(FILE *file, Vertex& vertex);
//	void ReadTaggedVector3(FILE *file, const char *tag, Vector3& vec);
//	void ReadVector3(FILE *file, Vector3& vec);
//	void ReadTaggedVector2(FILE *file, const char *tag, Vector2& vec);
//	void ReadVector2(FILE *file, Vector2& vec);
//	void ReadIndices(FILE *file, unsigned *&indices, unsigned& indicesCount);
//	unsigned int ReadIndicesCount(FILE *file);
//	void ReadIndicesTriangle(FILE *file, unsigned *indices);
//
//	const char *m_path;
//
//	static const char *const k_verticesCountTag;
//	static const char *const k_indicesCountTag;
//	static const char *const k_positionTag;
//	static const char *const k_normTag;
//	static const char *const k_binormTag;
//	static const char *const k_tangentTag;
//	static const char *const k_uvTag;
//
//};
//
//}
