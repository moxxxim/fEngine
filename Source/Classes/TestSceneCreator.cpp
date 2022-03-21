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
        std::map<Feng::Light *, std::unique_ptr<Feng::Material>> lightMaterials;
        
        std::array<Feng::Vector3, 10> cubePositions =
        {
            Feng::Vector3(-2.0f, 0.0f, 0.0f),
            Feng::Vector3( 0.0f,  0.0f,  0.0f),
            Feng::Vector3( 2.0f,  0.0f, 0.0f),
            Feng::Vector3(-2.0f, 2.0f, 0.0f),
            Feng::Vector3(0.0f, 2.0f, 0.0f),
            Feng::Vector3(2.0f, 2.0f, 0.0f),
            Feng::Vector3( -2.0f, 1.0f, -4.0f),
            Feng::Vector3( 0.0f, 1.0f, -4.0f),
            Feng::Vector3( 2.0f, 1.0f, -4.0f),
            Feng::Vector3( 0.0f, -3.0f, -2.0f),
        };
        
        std::array<Feng::Vector3, 10> vegetationPositions
        {
            Feng::Vector3(10.0f, -5.4f, 10.0f),
            Feng::Vector3(10.0f, -5.4f, -10.0f),
            Feng::Vector3(-10.0f, -5.4f, -10.0f),
            Feng::Vector3(-10.0f, -5.4f, 10.0f),
            Feng::Vector3(10.0f, -5.4f, 5.0f),
            Feng::Vector3(5.0f, -5.4f, 10.0f),
            Feng::Vector3(5.0f, -5.4f, 5.0f),
            Feng::Vector3(10.0f, -5.4f, 1.0f),
            Feng::Vector3(5.0f, -5.4f, 1.0f),
            Feng::Vector3(1.0f, -5.4f, 10.0f),
        };

        std::array<Feng::Vector3, 8> windowPositions
        {
            Feng::Vector3(10.0f, -5.4f, 20.0f),
            Feng::Vector3(20.0f, -5.4f, -10.0f),
            Feng::Vector3(-10.0f, -5.4f, -20.0f),
            Feng::Vector3(-20.0f, -5.4f, 10.0f),
            Feng::Vector3(0.0f, 0.0f, 2.0f),
            Feng::Vector3(1.0f, 2.0f, 2.0f),
            Feng::Vector3(-1.0f, -2.0f, 2.0f),
            Feng::Vector3(0.5f, 0.5f, 2.5f)
        };

        Feng::Vector3 planePos{0.f, -6.f, 0.f};
        
        Feng::Entity* CreateCamera(Feng::Scene& scene)
        {
            using namespace Feng;

            Entity &camEntity = scene.CreateCamera();

            Camera *camera = camEntity.GetComponent<Camera>();
            camera->SetFovY(45.f);
            camera->SetAspectRatio(static_cast<float>(Screen::ScreenWidth)/Screen::ScreenHeight);
            camera->SetNearClipPlane(0.1f);
            camera->SetFarClipPlane(100.f);

            Transform *camTransform = camEntity.GetComponent<Transform>();
            camTransform->SetPosition(0.f, 3.5f, 3.f);

            std::ignore = camEntity.AddComponent<CameraWasdController>();
            std::ignore = camEntity.AddComponent<CameraStabilizer>();

            return &camEntity;
        }

        std::unique_ptr<Feng::Material> CreateLightMaterial(const Feng::Vector4& color)
        {
            std::unique_ptr<Feng::Material> material = test::CreateGizmoMaterial(Feng::Engine::IsShowDepth());
            material->SetVector3(Feng::ShaderParams::MainColor.data(), color.GetXyz());

            return material;
        }

        void CreateDirectLight(Feng::Scene& scene)
        {
            using namespace Feng;
            
            Vector4 color {1.f, 0.95f, 0.8f, 1.f};

            std::unique_ptr<Material> material = CreateLightMaterial(color);
            Entity& lightEntity = scene.CreateLight(
                                                    Light::eType::Directional,
                                                    material.get(),
                                                    test::res.CubeMesh.get());

            Light* light = lightEntity.GetComponent<Light>();
            lightMaterials[light] = std::move(material);

            light->SetColor(color);
            light->SetIntensity(1.f);

            Transform *lightTransform = lightEntity.GetComponent<Transform>();
            lightTransform->SetPosition(0.f, 0.f, 4.f);
            lightTransform->SetRotation(Quaternion{Vector3::OneY, 180}, eSpace::World);
        }

        void CreatePointLight(Feng::Scene& scene)
        {
            using namespace Feng;
            
            Vector4 color {1.f, 0.f, 0.f, 1.f};

            std::unique_ptr<Material> material = CreateLightMaterial(color);
            Entity& lightEntity = scene.CreateLight(
                                                Light::eType::Point,
                                                material.get(),
                                                test::res.CubeMesh.get());

            Light* light = lightEntity.GetComponent<Light>();
            lightMaterials[light] = std::move(material);

            light->SetRange(8.f);
            light->SetColor(color);
            light->SetIntensity(1.5f);

            Transform *lightTransform = lightEntity.GetComponent<Transform>();

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

        void CreateSpotLight(Feng::Scene& scene)
        {
            using namespace Feng;
            
            Vector4 color {0.f, 0.f, 1.f, 1.f};

            std::unique_ptr<Material> material = CreateLightMaterial(color);
            Entity& lightEntity = scene.CreateLight(
                                                Light::eType::Spot,
                                                material.get(),
                                                test::res.CubeMesh.get());

            Light* light = lightEntity.GetComponent<Light>();
            lightMaterials[light] = std::move(material);

            light->SetRange(12.f);
            light->SetColor(color);
            light->SetIntensity(10.f);
            light->SetSpotAngle(Math::DegToRad(45));

            Transform *lightTransform = lightEntity.GetComponent<Transform>();
            lightTransform->SetPosition(5.f, 4.f, -2.f);
            lightTransform->SetRotation(Quaternion{Vector3::OneY, 90});

            GameObjectRotation& lightRotation = lightEntity.AddComponent<GameObjectRotation>();
            lightRotation.SetAxis(Feng::Vector3::OneX);
            lightRotation.SetPeriod(4.f);
        }

        Feng::Entity* CreateObject(
                                Feng::Scene& scene,
                                const Feng::Vector3& position,
                                const std::string& name,
                                Feng::Mesh &mesh,
                                Feng::Material& material)
        {
            Feng::Material *finalMaterial = Feng::Engine::IsShowDepth() ? test::res.ShowDepthMaterial.get() : &material;
            Feng::Entity& obj = scene.CreateMesh(finalMaterial, &mesh, name);

            Feng::Transform *transform = obj.GetComponent<Feng::Transform>();
            transform->SetPosition(position);

            return &obj;
        }
        
        void CreateInstancedObject(Feng::Scene& scene, Feng::Mesh &mesh)
        {
            Feng::Entity& obj = scene.CreateMesh(test::res.PhongTexInstancedMaterial.get(), &mesh, "Instanced");
            InstancedObjectTransformController &instances = obj.AddComponent<InstancedObjectTransformController>();
            instances.SetCount(10'000);
        }
        
        void CreateObjects(Feng::Scene& scene)
        {
            using namespace Feng;
            
            std::ignore = scene.CreateSkybox(test::res.SkyboxMaterial.get());
            Entity* camEntity = CreateCamera(scene);
            CreateDirectLight(scene);
            CreatePointLight(scene);
            CreateSpotLight(scene);
            CreateInstancedObject(scene, *test::res.CubeMesh);

            // Cubes.
            for(int32_t i = 0; i < cubePositions.size(); ++i)
            {
                const Vector3& position = cubePositions[i];
                std::string name = "cube " + std::to_string(i);
                Material *material = ((i % 2) == 0)
                    ? test::res.DiffTex1SpecTex2Material.get()
                    : test::res.SpecularTexMaterial.get();
                Entity* object = CreateObject(scene, position, name, *test::res.CubeMesh, *material);
                GameObjectRotation& objectRotation = object->AddComponent<GameObjectRotation>();
                objectRotation.SetAxis(Vector3::OneZ);
                objectRotation.SetPeriod(4.f);
            }

            // Grass.
            for(int32_t i = 0; i < vegetationPositions.size(); ++i)
            {
                const Vector3& position = vegetationPositions[i];
                std::string name = "grass " + std::to_string(i);
                std::ignore = CreateObject(scene, position, name, *test::res.QuadMesh, *test::res.GrassMaterial);
            }

            // Grass.
            for(int32_t i = 0; i < windowPositions.size(); ++i)
            {
                const Vector3& position = windowPositions[i];
                std::string name = "window " + std::to_string(i);
                std::ignore = CreateObject(scene, position, name, *test::res.QuadMesh, *test::res.WindowMaterial);
            }

            std::ignore = CreateObject(
                                    scene,
                                    Vector3(0.f, 2.f, -2.5f),
                                    "Reflective",
                                    *test::res.CubeMesh,
                                    *test::res.CubemapReflectiveMaterial);

            Entity *planeEntity = CreateObject(scene, planePos, "Plane", *test::res.CubeMesh, *test::res.PhongTexMaterial);
            Transform *planeTransform = planeEntity->GetComponent<Transform>();
            planeTransform->SetScale(40.f, 0.2f, 40.f);
            
            // Game manager.
            Entity &gameManger = scene.CreateEntity("Game Manger");
            gameManger.AddComponent<PostEffectSwitcher>();
        }
        
        std::unique_ptr<Feng::Scene> CreateScene()
        {
            std::unique_ptr<Feng::Scene> scene = std::make_unique<Feng::Scene>();
            scene->SetAmbientLight(Feng::Vector4{1.f, 1.f, 1.f, 1.f}, 0.2f);
            CreateObjects(*scene);

            return scene;
        }
    }
    
    std::unique_ptr<Feng::Scene> CreateTestScene()
    {
        static_assert(Feng::Quaternion::Identity().AxisScaled().x == 0);
        static_assert((Feng::Quaternion::Identity() * Feng::Quaternion::Identity()).AxisScaled().x == 0);
        static_assert((Feng::Vector3{0, 0, 1} * Feng::Quaternion::Identity()).x == 0);
        
        LoadResources();
        return CreateScene();
    }
}
