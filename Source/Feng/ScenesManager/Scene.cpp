#include <Feng/ScenesManager/Scene.h>

#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/MeshRenderer.h>
#include <Feng/ScenesManager/RenderSystem.h>
#include <Feng/ScenesManager/SceneSettings.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/ResourcesManager/Mesh.h>

#include <algorithm>

namespace Feng
{
    namespace SScene
    {
        std::vector<float> skyboxCube
        {
            // front
            -1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,

            // back
            -1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,

            // left
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,

            // right
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,

            // top
            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,

            // bottom
            -1.0f,  -1.0f, -1.0f,
            1.0f,  -1.0f, -1.0f,
            1.0f,  -1.0f,  1.0f,
            1.0f,  -1.0f,  1.0f,
            -1.0f,  -1.0f,  1.0f,
            -1.0f,  -1.0f, -1.0f
        };

        std::unique_ptr<Mesh> skyboxMesh;

        void AddLightGizmoRenderer(Light &light, Material *material, Mesh *mesh)
        {
            Entity *lightEntity = light.GetEntity();
            Transform *cubeTransform = lightEntity->GetComponent<Transform>();

            Light::eType lightType = light.GetType();

            if(lightType == Light::eType::Directional)
            {
                cubeTransform->SetScale(0.1f, 0.1f, 0.35f);
            }
            else if(lightType == Light::eType::Point)
            {
                cubeTransform->SetScale(0.2f);
            }
            else
            {
                cubeTransform->SetScale(0.35f, 0.35f, 0.1f);
            }

            MeshRenderer& meshRenderer = lightEntity->AddComponent<MeshRenderer>();
            meshRenderer.SetMesh(mesh);
            meshRenderer.SetMaterial(material);
        }
    }

    Scene::Scene()
        : renderSystem {std::make_unique<RenderSystem>()}
        , settings {std::make_unique<SceneSettings>()}
    { }

    Scene::~Scene() = default;

    Entity& Scene::CreateEntity()
    {
        std::unique_ptr<Entity> entity = std::make_unique<Entity>();
        entities.push_back(std::move(entity));
        return *entities.back();
    }

    Entity& Scene::CreateCamera()
    {
        Entity &camera = CreateEntity();
        camera.AddComponent<Camera>();

        renderSystem->SetCamera(camera.GetComponent<Camera>());
        return camera;
    }

    Entity& Scene::CreateLight(Light::eType type, Material *material, Mesh *mesh)
    {
        Entity &entity = CreateEntity();
        Light &light = entity.AddComponent<Light>();
        light.SetType(type);

        SScene::AddLightGizmoRenderer(light, material, mesh);

        renderSystem->AddLight(&light);
        renderSystem->AddRenderer(entity.GetComponent<MeshRenderer>());
        return entity;
    }

    Entity& Scene::CreateMesh(Material *material, Mesh *mesh)
    {
        Entity &entity = CreateEntity();
        MeshRenderer& renderer = entity.AddComponent<MeshRenderer>();
        renderer.SetMesh(mesh);
        renderer.SetMaterial(material);

        renderSystem->AddRenderer(&renderer);
        return entity;
    }

    Entity& Scene::CreateSkybox(Material *material)
    {
        SScene::skyboxMesh = std::make_unique<Mesh>(
                                            SScene::skyboxCube,
                                            eVertexAtributes::Position,
                                            ePrimitiveType::Triangles);

        Entity &skybox = CreateMesh(material, SScene::skyboxMesh.get());

        renderSystem->RemoveRenderer(skybox.GetComponent<MeshRenderer>()); // TODO: m.aleksee this is bad.
        renderSystem->SetSkybox(skybox.GetComponent<MeshRenderer>());

        return skybox;
    }

    void Scene::SetAmbientLight(Vector4 color, float intensity)
    {
        renderSystem->SetAmbientLight(color, intensity);
    }

    void Scene::SetPostEffect(PostEffectDefinition *postEffect)
    {
        renderSystem->SetPostEffect(postEffect);
    }

    void Scene::RemovePostEffect()
    {
        renderSystem->RemovePostEffect();
    }

    void Scene::Update(float deltaTime)
    {
        std::for_each(
                      entities.begin(),
                      entities.end(),
                      [deltaTime](const std::unique_ptr<Entity>& entity) { entity->Update(deltaTime); });
    }

    void Scene::Draw()
    {
        renderSystem->Draw();
    }
}
