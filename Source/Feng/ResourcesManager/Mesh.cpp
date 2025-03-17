#include <Feng/ResourcesManager/Mesh.h>

namespace Feng
{
    Mesh::Mesh(
               const std::vector<float>& aVertices,
               const std::vector<uint32_t>& aIndices,
               eVertexAtributes aAttributes,
               ePrimitiveType aPrimitives)
        : vertices { aVertices }
        , indices { aIndices }
        , attributes { aAttributes }
        , primitiveType { aPrimitives }
        , vertexStride { AttributesParser::GetStride(attributes) }
    { }

    Mesh::Mesh(
               std::vector<float>&& aVertices,
               std::vector<uint32_t>&& aIndices,
               eVertexAtributes aAttributes,
               ePrimitiveType aPrimitives)
        : vertices { std::move(aVertices) }
        , indices { std::move(aIndices) }
        , attributes { aAttributes }
        , primitiveType { aPrimitives }
        , vertexStride { AttributesParser::GetStride(attributes) }
    { }

    Mesh::Mesh(const std::vector<float>& aVertices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives)
        : vertices { aVertices }
        , attributes { aAttributes }
        , primitiveType { aPrimitives }
        , vertexStride { AttributesParser::GetStride(attributes) }
    { }

    Mesh::Mesh(std::vector<float>&& aVertices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives)
        : vertices { std::move(aVertices) }
        , attributes { aAttributes }
        , primitiveType { aPrimitives }
        , vertexStride { AttributesParser::GetStride(attributes) }
    { }
    
    bool Mesh::HasIndices() const
    {
        return !indices.empty();
    }

    eVertexAtributes Mesh::GetAttributes() const
    {
        return attributes;
    }

    ePrimitiveType Mesh::GetPrimitiveType() const
    {
        return primitiveType;
    }

    uint32_t Mesh::GetVertexStride() const
    {
        return vertexStride;
    }

    int32_t Mesh::GetVerticesCount() const
    {
        return static_cast<int32_t>(vertices.size() / vertexStride) * sizeof(float);
    }

    int32_t Mesh::GetIndicesCount() const
    {
        return static_cast<int32_t>(indices.size());
    }

    const std::vector<float>& Mesh::GetVertices() const
    {
        return vertices;
    }

    const std::vector<uint32_t>& Mesh::GetIndices() const
    {
        return indices;
    }

    uint32_t Mesh::GetDataSize() const
    {
        return static_cast<uint32_t>(vertices.size() * sizeof(float));
    }

    const uint8_t* Mesh::GetData() const
    {
        return reinterpret_cast<const uint8_t*>(vertices.data());
    }
}
