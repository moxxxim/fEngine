#pragma once

#include <Feng/ScenesManager/Light.h>

#include <memory>
#include <string>
#include <vector>

namespace Feng
{
    class Entity;
    class Material;
    class Mesh;
    class PostEffectDefinition;
    class RenderSystem;
    struct SceneSettings;

    class Scene final
    {
    public:
        Scene();
        ~Scene();

        Entity& CreateEntity(const std::string& name = "");
        Entity& CreateCamera();
        Entity& CreateLight(Light::eType type, Material *material, Mesh *mesh);
        Entity& CreateMesh(Material *material, Mesh *mesh, const std::string& name = "");
        Entity& CreateSkybox(Material *material);

        void SetAmbientLight(Vector4 color, float intensity);
        void SetPostEffect(PostEffectDefinition *postEffect);
        void RemovePostEffect();

        void Update(float deltaTime);
        void Draw();

    private:
        std::unique_ptr<RenderSystem> renderSystem;
        std::unique_ptr<SceneSettings> settings;
        std::vector<std::unique_ptr<Entity>> entities;
    };
}
