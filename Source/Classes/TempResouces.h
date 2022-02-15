#pragma once

#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Mesh.h>
#include <Feng/ResourcesManager/Texture.h>

#include <memory>
#include <vector>

namespace feng
{
    class Material;
    class Mesh;
    class PostEffectDefinition;
    class Texture;
    class TextureData;
}

namespace test
{
    struct TempRes final
    {
        std::unique_ptr<feng::Texture> WoodContainerTexture;
        std::unique_ptr<feng::Texture> BrickWallTexture;
        std::unique_ptr<feng::Texture> AwesomeFaceTexture;
        std::unique_ptr<feng::Texture> SteeledWoodTexture;
        std::unique_ptr<feng::Texture> SteelBorderTexture;
        std::unique_ptr<feng::Texture> GrassTexture;
        std::unique_ptr<feng::Texture> WindowTexture;
        std::unique_ptr<feng::Texture> SkyboxTexture;

        std::unique_ptr<feng::Material> DiffuseTexMaterial;
        std::unique_ptr<feng::Material> DiffuseTexInstancedMaterial;
        std::unique_ptr<feng::Material> SpecularTexMaterial;
        std::unique_ptr<feng::Material> DiffTex1SpecTex2Material;
        std::unique_ptr<feng::Material> ShowDepthMaterial;
        std::unique_ptr<feng::Material> GrassMaterial;
        std::unique_ptr<feng::Material> WindowMaterial;
        std::unique_ptr<feng::Material> SkyboxMaterial;
        std::unique_ptr<feng::Material> CubemapReflectiveMaterial;

        std::unique_ptr<feng::Mesh> CubeMesh;
        std::unique_ptr<feng::Mesh> QuadMesh;
        std::vector<feng::PostEffectDefinition*> Effects;
    };

    extern TempRes res;

    std::unique_ptr<feng::Material> CreateFlatColorMaterial();
    std::unique_ptr<feng::Material> CreateGizmoMaterial(bool showDepth);
    void LoadResources();
}
