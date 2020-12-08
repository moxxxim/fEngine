#include <FEngine/ResourcesManager/Mesh.h>

#include <FEngine/ResourcesManager/ResourceManager.h>

namespace feng
{
    Mesh::Mesh(const std::vector<float> &aData, const std::vector<int32_t> &aIndices, eVertexAtribute aAttributes)
        : data { aData }
        , indices { aIndices }
        , attributes { aAttributes }
    { }

    Mesh::Mesh(std::vector<float> &&aData, std::vector<int32_t> &&aIndices, eVertexAtribute aAttributes)
        : data { std::move(aData) }
        , indices { std::move(aIndices) }
        , attributes { aAttributes }
    { }
}
