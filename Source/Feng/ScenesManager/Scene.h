#pragma once

#include <Feng/ScenesManager/Light.h>

#include <memory>
#include <vector>

namespace feng
{
    class Entity;
    class Material;
    class Mesh;
    class RenderSystem;
    struct SceneSettings;

    class Scene final
    {
    public:
        Scene();
        ~Scene();

        Entity& CreateEntity();
        Entity& CreateCamera();
        Entity& CreateLight(Light::eType type, Material *material, Mesh *mesh);
        Entity& CreateMesh(Material *material, Mesh *mesh);
        Entity& CreateSkybox(Material *material);

        void Update(float deltaTime);
        void Draw();

    private:
//        void AddGameObject(GameObject *go);
//        void RemoveGameObject(GameObject *go);
//        void SetSkybox(Renderer *renderer);
//        void RemoveSkybox();

        std::unique_ptr<RenderSystem> renderSystem;
        std::unique_ptr<SceneSettings> settings;
        std::vector<std::unique_ptr<Entity>> entities;
    };
}
