#include <Classes/Scripts/InstancedObjectTransformController.hpp>

#include <Feng/Core/Engine.hpp>
#include <Feng/Math/Matrix3.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/MeshRenderer.h>

#include <random>

namespace
{
    static std::vector<Feng::Matrix4> InitializeInstances(size_t instancesCount)
    {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());
        std::uniform_real_distribution<float> angleDistribution(0.f, Feng::Math::Pi * 2);
        std::uniform_real_distribution<float> scaleDistribution(0.2f, 0.5f);
        std::uniform_real_distribution<float> radiusDistribution(20.f, 35.f);
        std::uniform_real_distribution<float> heightDistribution(3.f, 0.f);

        std::vector<Feng::Matrix4> objects;
        objects.resize(instancesCount);

        for(size_t i = 0; i < instancesCount; ++i)
        {
            float scaleValue = scaleDistribution(generator);
            Feng::Vector3 scale { scaleValue, scaleValue, scaleValue };

            float angle = angleDistribution(generator);
            float radius = radiusDistribution(generator);
            float height = heightDistribution(generator);
            Feng::Vector3 translation = radius
                                        * (Feng::Vector3::OneX * std::sin(angle) + Feng::Vector3::OneZ * std::cos(angle))
                                        + height * Feng::Vector3::OneY;

            objects[i] = Feng::Mat4::MakeTransformation(scale, translation, Feng::Quaternion::Identity());
        }

        return objects;
    }
}

void InstancedObjectTransformController::SetCount(size_t count)
{
    instances = InitializeInstances(count);
}

void InstancedObjectTransformController::Update(float)
{
    if(!renderer)
    {
        Feng::Entity *entity = GetEntity();
        renderer = entity->GetComponent<Feng::MeshRenderer>();
    }

    constexpr float offsetFactor = 0.01f;
    constexpr float speedFactor = 0.1f;
    for(size_t i = 0; i < instances.size(); i+=2)
    {
        instances[i].rows[3].coord.z += offsetFactor * std::sin(speedFactor * Feng::Engine::Time());
    }

    renderer->SetInstanceTransforms(instances);
}
