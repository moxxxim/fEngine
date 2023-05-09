#pragma once

#include <Feng/Utils/Render/MeshParams.h>

#include <cstdint>
#include <vector>

namespace Feng
{
    class Mesh
    {
    public:
        Mesh(
             const std::vector<float>& aVertices,
             const std::vector<uint32_t>& aIndices,
             eVertexAtributes aAttributes,
             ePrimitiveType aPrimitives);
        Mesh(
             std::vector<float>&& aVertices,
             std::vector<uint32_t>&& aIndices,
             eVertexAtributes aAttributes,
             ePrimitiveType aPrimitives);

        Mesh(const std::vector<float>& aVertices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives);
        Mesh(std::vector<float>&& aVertices, eVertexAtributes aAttributes, ePrimitiveType aPrimitives);

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
}
