#pragma once

#include <Feng/ScenesManager/Component.h>
#include <Feng/Math/Matrix4.h>

#include <vector>

namespace Feng
{
    class MeshRenderer;
}

class InstancedObjectTransformController final : public Feng::Component
{
public:
    InstancedObjectTransformController() = default;
    
    void SetCount(size_t count);
    
private:
    void Update(float deltaTime) override;
    
    std::vector<Feng::Matrix4> instances;
    Feng::MeshRenderer *renderer = nullptr;
};
