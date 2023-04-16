#include <Classes/TestSceneCreator.hpp>

#include <Classes/Scripts/CameraStabilizer.hpp>
#include <Classes/Scripts/CameraWasdController.h>
#include <Classes/Scripts/DirectionalLightMovement.hpp>
#include <Classes/Scripts/GameObjectRotation.h>
#include <Classes/Scripts/GameObjectSliderX.hpp>
#include <Classes/Scripts/InstancedObjectTransformController.hpp>
#include <Classes/Scripts/FeatureSwitcher.hpp>
#include <Classes/TempResouces.h>

#include <Feng/App/Globals.h>
#include <Feng/Core/Engine.hpp>
#include <Feng/Math/MathUtils.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/Math/Vector4.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/MeshRenderer.h>
#include <Feng/ScenesManager/RenderSystem.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/Utils/Render/ShaderParams.h>

namespace test
{
    namespace
    {
        std::map<Feng::Light *, std::unique_ptr<Feng::Material>> lightMaterials;
        
        constexpr Feng::Vector3 planePos{0.f, -1.f, 0.f};
        constexpr Feng::Vector3 reflectiveCubePos{6.f, planePos.y + 0.6f, 0.f};
        
        std::vector<Feng::Vector3> cubePositions =
        {
            Feng::Vector3(-4.0f, 0.0f, 0.0f),
            Feng::Vector3( 0.0f,  planePos.y + 0.6f,  -5.5f),
            Feng::Vector3( 2.0f,  0.0f, 0.0f),
            Feng::Vector3(-2.0f, 2.0f, 0.0f),
            Feng::Vector3(0.0f, 2.0f, 0.0f),
            Feng::Vector3(2.0f, 2.0f, 0.0f),
            Feng::Vector3( -2.0f, 1.0f, -4.0f),
            Feng::Vector3( 0.0f, 1.0f, -4.0f),
            Feng::Vector3( 2.0f, 1.0f, -4.0f),
            Feng::Vector3( -2.f, planePos.y + 0.6f, -1.f),
        };

        std::array<Feng::Vector3, 10> vegetationPositions
        {
            Feng::Vector3(10.0f, planePos.y + 0.6f, 10.0f),
            Feng::Vector3(10.0f, planePos.y + 0.6f, -10.0f),
            Feng::Vector3(-10.0f, planePos.y + 0.6f, -10.0f),
            Feng::Vector3(-10.0f, planePos.y + 0.6f, 10.0f),
            Feng::Vector3(10.0f, planePos.y + 0.6f, 5.0f),
            Feng::Vector3(5.0f, planePos.y + 0.6f, 10.0f),
            Feng::Vector3(5.0f, planePos.y + 0.6f, 5.0f),
            Feng::Vector3(10.0f, planePos.y + 0.6f, 1.0f),
            Feng::Vector3(5.0f, planePos.y + 0.6f, 1.0f),
            Feng::Vector3(1.0f, planePos.y + 0.6f, 10.0f),
        };

        std::array<Feng::Vector3, 8> windowPositions
        {
            Feng::Vector3(10.0f, planePos.y + 0.6f, 20.0f),
            Feng::Vector3(20.0f, planePos.y + 0.6f, -10.0f),
            Feng::Vector3(-10.0f, planePos.y + 0.6f, -20.0f),
            Feng::Vector3(-20.0f, planePos.y + 0.6f, 10.0f),
            Feng::Vector3(0.0f, 0.0f, 2.0f),
            Feng::Vector3(1.0f, 2.0f, 2.0f),
            Feng::Vector3(-1.0f, -0.2f, 2.0f),
            Feng::Vector3(0.5f, 0.5f, 2.5f)
        };

        Feng::Entity* CreateCamera(Feng::Scene& scene)
        {
            using namespace Feng;

            Entity &camEntity = scene.CreateCamera();

            Camera *camera = camEntity.GetComponent<Camera>();
            camera->SetFovY(45.f);
            camera->SetAspectRatio(static_cast<float>(Screen::ScreenSize.width)/Screen::ScreenSize.height);
            camera->SetNearClipPlane(0.1f);
            camera->SetFarClipPlane(50.f);

            Transform *camTransform = camEntity.GetComponent<Transform>();
            camTransform->SetPosition(0.f, 3.5f, 3.f);

            std::ignore = camEntity.AddComponent<CameraWasdController>();
            CameraStabilizer& stabilizer = camEntity.AddComponent<CameraStabilizer>();
            stabilizer.SetMinMaxFov(20.f, 60.f);
            
            return &camEntity;
        }

        std::unique_ptr<Feng::Material> CreateLightMaterial(const Feng::Vector4& color)
        {
            std::unique_ptr<Feng::Material> material = test::CreateGizmoMaterial(Feng::Engine::IsShowDepth());
            material->Bindings().SetVector3(Feng::ShaderParams::MainColor.data(), color.GetXyz());

            return material;
        }

        void CreateDirectLight(Feng::Scene& scene, bool isShadowCaster)
        {
            using namespace Feng;

            Vector4 color {1.f, 0.95f, 0.8f, 1.f};

            std::unique_ptr<Material> material = CreateLightMaterial(color);
            Entity& lightEntity = scene.CreateLight(
                                                    Light::eType::Directional,
                                                    material.get(),
                                                    test::res.CubeMesh.get());
            //lightEntity.AddComponent<DirectionalLightMovement>();

            Light* light = lightEntity.GetComponent<Light>();
            lightMaterials[light] = std::move(material);

            light->SetColor(color);
            light->SetIntensity(1.f);
            light->SetShadowCaster(isShadowCaster);
            
            scene.GetRenderSystem()->SetDirectionalShadowLight(&lightEntity);

            Transform *lightTransform = lightEntity.GetComponent<Transform>();
            lightTransform->SetPosition(-7, 3, 5);
            lightTransform->SetEuler(-40.f, -40, 0.f);
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

            light->SetRange(20.f);
            light->SetColor(color);
            light->SetIntensity(3.f);
            
            scene.GetRenderSystem()->SetPointShadowLight(&lightEntity);

            //std::ignore = lightEntity.AddComponent<GameObjectSliderX>();
            Transform *lightTransform = lightEntity.GetComponent<Transform>();
            lightTransform->SetPosition(0.f, 1.f, -2.f);
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

//            GameObjectRotation& lightRotation = lightEntity.AddComponent<GameObjectRotation>();
//            lightRotation.SetAxis(Feng::Vector3::OneX);
//            lightRotation.SetPeriod(4.f);
        }

        Feng::Entity* CreateObject(
                                Feng::Scene& scene,
                                const Feng::Vector3& position,
                                const std::string& name,
                                Feng::Mesh &mesh,
                                Feng::Material& material,
                                bool shadowCaster)
        {
            Feng::Material *finalMaterial = Feng::Engine::IsShowDepth() ? test::res.ShowDepthMaterial.get() : &material;
            Feng::Entity& obj = scene.CreateMesh(finalMaterial, &mesh, name);
            Feng::MeshRenderer* renderer = obj.GetComponent<Feng::MeshRenderer>();
            renderer->SetShadowCaster(shadowCaster);

            Feng::Transform *transform = obj.GetComponent<Feng::Transform>();
            transform->SetPosition(position);

            return &obj;
        }

        void CreateInstancedObject(Feng::Scene& scene, Feng::Mesh &mesh)
        {
            Feng::Entity& obj = scene.CreateMesh(test::res.PhongTexInstancedMaterial.get(), &mesh, "Instanced");
            InstancedObjectTransformController &instances = obj.AddComponent<InstancedObjectTransformController>();
            instances.SetCount(1'000);
        }
        
        void CreateObjects(Feng::Scene& scene)
        {
            using namespace Feng;
            
            std::ignore = scene.CreateSkybox(test::res.SkyboxMaterial.get());
            Entity* camEntity = CreateCamera(scene);
            CreateDirectLight(scene, true);
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
                Entity* object = CreateObject(scene, position, name, *test::res.CubeMesh, *material, true);
                if ((i % 2) == 0)
                {
                    GameObjectRotation& objectRotation = object->AddComponent<GameObjectRotation>();
                    objectRotation.SetAxis(Vector3::OneZ);
                    objectRotation.SetPeriod(4.f);
                }
            }
            
            // Plane.
            Entity *planeEntity = CreateObject(scene, planePos, "Plane", *test::res.CubeMesh, *test::res.PhongTexMaterial, false);
            Transform *planeTransform = planeEntity->GetComponent<Transform>();
            planeTransform->SetScale(40.f, 0.2f, 40.f);

            // Grass.
            for(int32_t i = 0; i < vegetationPositions.size(); ++i)
            {
                const Vector3& position = vegetationPositions[i];
                std::string name = "grass " + std::to_string(i);
                std::ignore = CreateObject(scene, position, name, *test::res.QuadMesh, *test::res.GrassMaterial, false);
            }

            // Windows.
            for(int32_t i = 0; i < windowPositions.size(); ++i)
            {
                const Vector3& position = windowPositions[i];
                std::string name = "window " + std::to_string(i);
                std::ignore = CreateObject(scene, position, name, *test::res.QuadMesh, *test::res.WindowMaterial, false);
            }

            Feng::Entity *reflectiveeObject = CreateObject(
                                                          scene,
                                                          reflectiveCubePos,
                                                          "Reflective",
                                                          *test::res.CubeMesh,
                                                          *test::res.CubemapReflectiveMaterial,
                                                          true);
            Feng::Transform *reflectiveObjectTransform = reflectiveeObject->GetComponent<Feng::Transform>();

            // Game manager.
            Entity &gameManger = scene.CreateEntity("Game Manger");
            gameManger.AddComponent<FeatureSwitcher>();
        }
        
        std::unique_ptr<Feng::Scene> CreateScene()
        {
            std::unique_ptr<Feng::Scene> scene = std::make_unique<Feng::Scene>();
            scene->GetRenderSystem()->SetAmbientLight(Feng::Vector4{1.f, 1.f, 1.f, 1.f}, 0.2f);
            scene->GetRenderSystem()->SetShadowMaterials(
                                                         res.DirectShadowPassMaterial.get(),
                                                         res.OmniShadowPassMaterial.get(),
                                                         res.ShadowMapDrawMaterial.get());
            scene->GetRenderSystem()->SetCascadeBorders({0.33f, 0.66f});
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
