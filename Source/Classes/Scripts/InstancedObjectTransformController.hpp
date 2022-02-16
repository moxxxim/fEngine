#pragma once

#include <Feng/ScenesManager/Component.h>
#include <Feng/Math/Matrix4.h>

#include <vector>

namespace feng
{
    class MeshRenderer;
}

class InstancedObjectTransformController final : public feng::Component
{
public:
    InstancedObjectTransformController() = default;
    
    void SetCount(size_t count);
    
private:
    void Update(float deltaTime) override;
    
    std::vector<feng::Matrix4> instances;
    feng::MeshRenderer *renderer = nullptr;
};
