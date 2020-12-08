#pragma once

#include <cstdint>
#include <vector>

namespace feng
{
    enum eVertexAtribute : uint32_t
    {
        Position = 1 << 0,
        Normal = 1 << 1,
        Tangent = 1 << 2,
        Binormal = 1 << 3,
        Uv1 = 1 << 4,
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<float> &aData, const std::vector<int32_t> &aIndices, eVertexAtribute aAttributes);
        Mesh(std::vector<float> &&aData, std::vector<int32_t> &&aIndices, eVertexAtribute aAttributes);

        int32_t GetVerticesCount() const;
        int32_t GetIndicesCount() const;
        const std::vector<float>& GetData() const;
        const std::vector<int32_t>& GetIndices() const;

    private:
        std::vector<float> data;
        std::vector<int32_t> indices;
        eVertexAtribute attributes;
    };

    inline int32_t Mesh::GetVerticesCount() const
    {
        return static_cast<int32_t>(data.size());
    }

    inline int32_t Mesh::GetIndicesCount() const
    {
        return static_cast<int32_t>(indices.size());
    }

    inline const std::vector<float>& Mesh::GetData() const
    {
        return data;
    }

    inline const std::vector<int32_t>& Mesh::GetIndices() const
    {
        return indices;
    }
}
