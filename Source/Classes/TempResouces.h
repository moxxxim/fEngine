#pragma once

#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Mesh.h>
#include <Feng/ResourcesManager/Texture.h>

#include <memory>
#include <vector>

namespace Feng
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
        std::unique_ptr<Feng::Texture> WoodContainerTexture;
        std::unique_ptr<Feng::Texture> BrickWallTexture;
        std::unique_ptr<Feng::Texture> AwesomeFaceTexture;
        std::unique_ptr<Feng::Texture> SteeledWoodTexture;
        std::unique_ptr<Feng::Texture> SteelBorderTexture;
        std::unique_ptr<Feng::Texture> GrassTexture;
        std::unique_ptr<Feng::Texture> WindowTexture;
        std::unique_ptr<Feng::Texture> SkyboxTexture;

        std::unique_ptr<Feng::Material> PhongTexMaterial;
        std::unique_ptr<Feng::Material> PhongTexInstancedMaterial;
        std::unique_ptr<Feng::Material> SpecularTexMaterial;
        std::unique_ptr<Feng::Material> DiffTex1SpecTex2Material;
        std::unique_ptr<Feng::Material> ShowDepthMaterial;
        std::unique_ptr<Feng::Material> GrassMaterial;
        std::unique_ptr<Feng::Material> WindowMaterial;
        std::unique_ptr<Feng::Material> SkyboxMaterial;
        std::unique_ptr<Feng::Material> CubemapReflectiveMaterial;
        std::unique_ptr<Feng::Material> CubemapRefractionMaterial;

        std::unique_ptr<Feng::Mesh> CubeMesh;
        std::unique_ptr<Feng::Mesh> QuadMesh;
        std::vector<Feng::PostEffectDefinition*> Effects;
    };

    extern TempRes res;

    std::unique_ptr<Feng::Material> CreateFlatColorMaterial();
    std::unique_ptr<Feng::Material> CreateGizmoMaterial(bool showDepth);
    void LoadResources();
}
