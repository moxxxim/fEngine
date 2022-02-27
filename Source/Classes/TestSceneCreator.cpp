#include <Classes/TestSceneCreator.hpp>

#include <Classes/Scripts/CameraStabilizer.hpp>
#include <Classes/Scripts/CameraWasdController.h>
#include <Classes/Scripts/GameObjectRotation.h>
#include <Classes/Scripts/InstancedObjectTransformController.hpp>
#include <Classes/Scripts/PostEffectSwitcher.hpp>
#include <Classes/TempResouces.h>

#include <Feng/App/Globals.h>
#include <Feng/Core/Engine.hpp>
#include <Feng/Math/MathUtils.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/Math/Vector4.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/MeshRenderer.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/Utils/Render/ShaderParams.h>

namespace test
{
    namespace
    {
        std::map<feng::Light *, std::unique_ptr<feng::Material>> lightMaterials;
        
        std::array<feng::Vector3, 10> cubePositions =
        {
            feng::Vector3(-2.0f, 0.0f, 0.0f),
            feng::Vector3( 0.0f,  0.0f,  0.0f),
            feng::Vector3( 2.0f,  0.0f, 0.0f),
            feng::Vector3(-2.0f, 2.0f, 0.0f),
            feng::Vector3(0.0f, 2.0f, 0.0f),
            feng::Vector3(2.0f, 2.0f, 0.0f),
            feng::Vector3( -2.0f, 1.0f, -4.0f),
            feng::Vector3( 0.0f, 1.0f, -4.0f),
            feng::Vector3( 2.0f, 1.0f, -4.0f),
            feng::Vector3( 0.0f, -3.0f, -2.0f),
        };
        
        std::array<feng::Vector3, 10> vegetationPositions
        {
            feng::Vector3(10.0f, -5.4f, 10.0f),
            feng::Vector3(10.0f, -5.4f, -10.0f),
            feng::Vector3(-10.0f, -5.4f, -10.0f),
            feng::Vector3(-10.0f, -5.4f, 10.0f),
            feng::Vector3(10.0f, -5.4f, 5.0f),
            feng::Vector3(5.0f, -5.4f, 10.0f),
            feng::Vector3(5.0f, -5.4f, 5.0f),
            feng::Vector3(10.0f, -5.4f, 1.0f),
            feng::Vector3(5.0f, -5.4f, 1.0f),
            feng::Vector3(1.0f, -5.4f, 10.0f),
        };

        std::array<feng::Vector3, 8> windowPositions
        {
            feng::Vector3(10.0f, -5.4f, 20.0f),
            feng::Vector3(20.0f, -5.4f, -10.0f),
            feng::Vector3(-10.0f, -5.4f, -20.0f),
            feng::Vector3(-20.0f, -5.4f, 10.0f),
            feng::Vector3(0.0f, 0.0f, 2.0f),
            feng::Vector3(1.0f, 2.0f, 2.0f),
            feng::Vector3(-1.0f, -2.0f, 2.0f),
            feng::Vector3(0.5f, 0.5f, 2.5f)
        };

        feng::Vector3 planePos{0.f, -6.f, 0.f};
        
        feng::Entity* CreateCamera(feng::Scene& scene)
        {
            feng::Entity &camEntity = scene.CreateCamera();

            feng::Camera *camera = camEntity.GetComponent<feng::Camera>();
            camera->SetFovY(45.f);
            camera->SetAspectRatio(static_cast<float>(feng::screen::ScreenWidth)/feng::screen::ScreenHeight);
            camera->SetNearClipPlane(0.1f);
            camera->SetFarClipPlane(100.f);

            feng::Transform *camTransform = camEntity.GetComponent<feng::Transform>();
            camTransform->SetPosition(0.f, 3.5f, 3.f);

            std::ignore = camEntity.AddComponent<CameraWasdController>();
            std::ignore = camEntity.AddComponent<CameraStabilizer>();

            return &camEntity;
        }

        std::unique_ptr<feng::Material> CreateLightMaterial(const feng::Vector4& color)
        {
            std::unique_ptr<feng::Material> material = test::CreateGizmoMaterial(feng::Engine::IsShowDepth());
            material->SetVector3(feng::ShaderParams::MainColor.data(), color.GetXyz());

            return material;
        }

        void CreateDirectLight(feng::Scene& scene)
        {
            feng::Vector4 color {1.f, 0.95f, 0.8f, 1.f};

            std::unique_ptr<feng::Material> material = CreateLightMaterial(color);
            feng::Entity& lightEntity = scene.CreateLight(
                                                        feng::Light::eType::Directional,
                                                        material.get(),
                                                        test::res.CubeMesh.get());

            feng::Light* light = lightEntity.GetComponent<feng::Light>();
            lightMaterials[light] = std::move(material);

            light->SetColor(color);
            light->SetIntensity(1.f);

            feng::Transform *lightTransform = lightEntity.GetComponent<feng::Transform>();
            lightTransform->SetPosition(0.f, 0.f, 4.f);
            lightTransform->SetRotation(feng::Quaternion{feng::Vector3::OneY, 180}, feng::eSpace::World);
        }

        void CreatePointLight(feng::Scene& scene)
        {
            feng::Vector4 color {1.f, 0.f, 0.f, 1.f};

            std::unique_ptr<feng::Material> material = CreateLightMaterial(color);
            feng::Entity& lightEntity = scene.CreateLight(
                                                            feng::Light::eType::Point,
                                                            material.get(),
                                                            test::res.CubeMesh.get());

            feng::Light* light = lightEntity.GetComponent<feng::Light>();
            lightMaterials[light] = std::move(material);

            light->SetRange(8.f);
            light->SetColor(color);
            light->SetIntensity(1.5f);

            feng::Transform *lightTransform = lightEntity.GetComponent<feng::Transform>();

            constexpr bool isUp = true;
            if(isUp)
            {
                lightTransform->SetPosition(0.f, 4.f, -1.f);
            }
            else
            {
                lightTransform->SetPosition(0.f, -12.f, -1.f);
            }
        }

        void CreateSpotLight(feng::Scene& scene)
        {
            feng::Vector4 color {0.f, 0.f, 1.f, 1.f};

            std::unique_ptr<feng::Material> material = CreateLightMaterial(color);
            feng::Entity& lightEntity = scene.CreateLight(
                                                            feng::Light::eType::Spot,
                                                            material.get(),
                                                            test::res.CubeMesh.get());

            feng::Light* light = lightEntity.GetComponent<feng::Light>();
            lightMaterials[light] = std::move(material);

            light->SetRange(12.f);
            light->SetColor(color);
            light->SetIntensity(10.f);
            light->SetSpotAngle(feng::DegToRad(45));

            feng::Transform *lightTransform = lightEntity.GetComponent<feng::Transform>();
            constexpr bool isUp = true;

            if(isUp)
            {
                lightTransform->SetPosition(5.f, 4.f, -2.f);
                lightTransform->SetRotation(feng::Quaternion(feng::Vector3::OneY, 90), feng::eSpace::World);
                lightTransform->SetRotation(feng::Quaternion(feng::Vector3::OneX, 20), feng::eSpace::Self);
            }
            else
            {
                lightTransform->SetPosition(0.f, -8.f, 2.f);
                lightTransform->SetRotation(feng::Quaternion(feng::Vector3::OneX, -90), feng::eSpace::Self);
            }
        }

        feng::Entity* CreateObject(
                                feng::Scene& scene,
                                const feng::Vector3& position,
                                const std::string& name,
                                feng::Mesh &mesh,
                                feng::Material& material)
        {
            feng::Material *finalMaterial = feng::Engine::IsShowDepth() ? test::res.ShowDepthMaterial.get() : &material;
            feng::Entity& obj = scene.CreateMesh(finalMaterial, &mesh);

            feng::Transform *transform = obj.GetComponent<feng::Transform>();
            transform->SetPosition(position);

            return &obj;
        }
        
        void CreateInstancedObject(feng::Scene& scene, feng::Mesh &mesh)
        {
            feng::Entity& obj = scene.CreateMesh(test::res.DiffuseTexInstancedMaterial.get(), &mesh);
            InstancedObjectTransformController &instances = obj.AddComponent<InstancedObjectTransformController>();
            instances.SetCount(10'000);
        }
        
        void CreateObjects(feng::Scene& scene)
        {
            std::ignore = scene.CreateSkybox(test::res.SkyboxMaterial.get());
            feng::Entity* camEntity = CreateCamera(scene);
            CreateDirectLight(scene);
            CreatePointLight(scene);
            CreateSpotLight(scene);
            CreateInstancedObject(scene, *test::res.CubeMesh);

            // Cubes.
            for(int32_t i = 0; i < cubePositions.size(); ++i)
            {
                const feng::Vector3& position = cubePositions[i];
                std::string name = "cube " + std::to_string(i);
                feng::Material *material = ((i % 2) == 0)
                    ? test::res.DiffTex1SpecTex2Material.get()
                    : test::res.SpecularTexMaterial.get();
                feng::Entity* object = CreateObject(scene, position, name, *test::res.CubeMesh, *material);
                object->AddComponent<GameObjectRotation>();
            }

            // Grass.
            for(int32_t i = 0; i < vegetationPositions.size(); ++i)
            {
                const feng::Vector3& position = vegetationPositions[i];
                std::string name = "grass " + std::to_string(i);
                std::ignore = CreateObject(scene, position, name, *test::res.QuadMesh, *test::res.GrassMaterial);
            }

            // Grass.
            for(int32_t i = 0; i < windowPositions.size(); ++i)
            {
                const feng::Vector3& position = windowPositions[i];
                std::string name = "window " + std::to_string(i);
                std::ignore = CreateObject(scene, position, name, *test::res.QuadMesh, *test::res.WindowMaterial);
            }

            std::ignore = CreateObject(
                                    scene,
                                    feng::Vector3(0.f, 2.f, -2.5f),
                                    "Reflective",
                                    *test::res.CubeMesh,
                                    *test::res.CubemapReflectiveMaterial);

            feng::Entity *planeEntity = CreateObject(scene, planePos, "Plane", *test::res.CubeMesh, *test::res.DiffuseTexMaterial);
            feng::Transform *planeTransform = planeEntity->GetComponent<feng::Transform>();
            planeTransform->SetScale(40.f, 0.2f, 40.f);
            
            // Game manager.
            feng::Entity &gameManger = scene.CreateEntity();
            gameManger.AddComponent<PostEffectSwitcher>();
        }
        
        std::unique_ptr<feng::Scene> CreateScene()
        {
            std::unique_ptr<feng::Scene> scene = std::make_unique<feng::Scene>();
            scene->SetAmbientLight(feng::Vector4{1.f, 1.f, 1.f, 1.f}, 0.2f);
            CreateObjects(*scene);

            return scene;
        }
    }
    
    std::unique_ptr<feng::Scene> CreateTestScene()
    {
        static_assert(feng::Quaternion::Identity().AxisScaled().x == 0);
        static_assert((feng::Quaternion::Identity() * feng::Quaternion::Identity()).AxisScaled().x == 0);
        static_assert((feng::Vector3{0, 0, 1} * feng::Quaternion::Identity()).x == 0);
        
        LoadResources();
        return CreateScene();
    }
}
