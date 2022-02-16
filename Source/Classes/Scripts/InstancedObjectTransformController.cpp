#include <Classes/Scripts/InstancedObjectTransformController.hpp>

#include <Feng/Core/Engine.hpp>
#include <Feng/Math/Matrix3.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/MeshRenderer.h>

#include <random>

namespace
{
    static std::vector<feng::Matrix4> InitializeInstances(size_t instancesCount)
    {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());
        std::uniform_real_distribution<float> angleDistribution(0.f, 359.9f);
        std::uniform_real_distribution<float> scaleDistribution(0.2f, 0.5f);
        std::uniform_real_distribution<float> radiusDistribution(20.f, 35.f);
        std::uniform_real_distribution<float> heightDistribution(1.5f, -1.5f);

        std::vector<feng::Matrix4> objects;
        objects.resize(instancesCount);

        for(size_t i = 0; i < instancesCount; ++i)
        {
            float scaleValue = scaleDistribution(generator);
            feng::Vector3 scale { scaleValue, scaleValue, scaleValue };

            float angle = angleDistribution(generator);
            float radius = radiusDistribution(generator);
            float height = heightDistribution(generator);
            feng::Vector3 translation = radius
                                        * (feng::Vector3::OneX * std::sin(angle) + feng::Vector3::OneZ * std::cos(angle))
                                        + height * feng::Vector3::OneY;

            objects[i] = feng::mat4::MakeTransformation(scale, feng::Matrix3::Identity, translation);
        }

        return objects;
    }
}

void InstancedObjectTransformController::SetCount(size_t count)
{
    instances = InitializeInstances(count);
}

void InstancedObjectTransformController::Update(float deltaTime)
{
    if(!renderer)
    {
        feng::Entity *entity = GetEntity();
        renderer = entity->GetComponent<feng::MeshRenderer>();
    }

    constexpr float offsetFactor = 0.1f;
    for(size_t i = 0; i < instances.size(); i+=2)
    {
        instances[i].rows[3].z += offsetFactor * std::sin(feng::Engine::Time());
    }

    renderer->SetInstanceTransforms(instances);
}
