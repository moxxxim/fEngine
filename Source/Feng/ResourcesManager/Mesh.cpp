#include <Feng/ResourcesManager/Mesh.h>

#include <Feng/ResourcesManager/ResourceManager.h>

namespace feng
{
    Mesh::Mesh(const std::vector<float> &aVertices, const std::vector<uint32_t> &aIndices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives)
        : vertices { aVertices }
        , indices { aIndices }
        , attributes { aAttributes }
        , primitiveType { aPrimitives }
        , vertexStride { AttributesParser::GetStride(attributes) }
    { }

    Mesh::Mesh(std::vector<float> &&aVertices, std::vector<uint32_t> &&aIndices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives)
        : vertices { std::move(aVertices) }
        , indices { std::move(aIndices) }
        , attributes { aAttributes }
        , primitiveType { aPrimitives }
        , vertexStride { AttributesParser::GetStride(attributes) }
    { }

    Mesh::Mesh(const std::vector<float> &aVertices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives)
        : vertices { aVertices }
        , attributes { aAttributes }
        , primitiveType { aPrimitives }
        , vertexStride { AttributesParser::GetStride(attributes) }
    { }

    Mesh::Mesh(std::vector<float> &&aVertices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives)
        : vertices { std::move(aVertices) }
        , attributes { aAttributes }
        , primitiveType { aPrimitives }
        , vertexStride { AttributesParser::GetStride(attributes) }
    { }
}
