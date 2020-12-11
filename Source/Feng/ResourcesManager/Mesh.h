#pragma once

#include <Feng/Utils/Render/MeshParams.h>

#include <cstdint>
#include <vector>

namespace feng
{
    class Mesh
    {
    public:
        Mesh(const std::vector<float> &aVertices, const std::vector<uint32_t> &aIndices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives);
        Mesh(std::vector<float> &&aVertices, std::vector<uint32_t> &&aIndices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives);

        Mesh(const std::vector<float> &aVertices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives);
        Mesh(std::vector<float> &&aVertices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives);

        bool HasIndices() const;
        eVertexAtributes GetAttributes() const;
        ePrimitiveType GetPrimitiveType() const;

        uint32_t GetVertexStride() const;
        int32_t GetVerticesCount() const;
        int32_t GetIndicesCount() const;
        const std::vector<float>& GetVertices() const;
        const std::vector<uint32_t>& GetIndices() const;

        uint32_t GetDataSize() const;
        const uint8_t* GetData() const;

    private:
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        eVertexAtributes attributes = eVertexAtributes::Empty;
        ePrimitiveType primitiveType = ePrimitiveType::Triangles;
        uint32_t vertexStride = 0;
    };

    inline bool Mesh::HasIndices() const
    {
        return !indices.empty();
    }

    inline eVertexAtributes Mesh::GetAttributes() const
    {
        return attributes;
    }

    inline ePrimitiveType Mesh::GetPrimitiveType() const
    {
        return primitiveType;
    }

    inline uint32_t Mesh::GetVertexStride() const
    {
        return vertexStride;
    }

    inline int32_t Mesh::GetVerticesCount() const
    {
        return static_cast<int32_t>(vertices.size() / vertexStride);
    }

    inline int32_t Mesh::GetIndicesCount() const
    {
        return static_cast<int32_t>(indices.size());
    }

    inline const std::vector<float>& Mesh::GetVertices() const
    {
        return vertices;
    }

    inline const std::vector<uint32_t>& Mesh::GetIndices() const
    {
        return indices;
    }

    inline uint32_t Mesh::GetDataSize() const
    {
        return vertices.size() * sizeof(float);
    }

    inline const uint8_t* Mesh::GetData() const
    {
        return reinterpret_cast<const uint8_t*>(vertices.data());
    }
}
