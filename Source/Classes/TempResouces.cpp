#include <Classes/TempResouces.h>

#include <Feng/Render/RenderBase.h>
#include <Feng/Render/PostEffects/Core/PostEffectDefinition.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/ResourcesManager/TextureData.h>
#include <Feng/Utils/Render/ShaderParams.h>

#include <array>
#include <cstdint>
#include <string>

namespace
{
    std::string BaseResourcesDir = "../../../Resources/";
    std::string BaseTexturesDir = BaseResourcesDir + "Textures/";
    std::string BaseShadersDir = BaseResourcesDir + "Shaders/";

    constexpr const char *FlatColorFsName = "Unlit/UnlitFlatColorFs.fs";
    constexpr const char *FlatColorVsName = "Unlit/UnlitFlatColorVs.vs";
    constexpr const char *UnlitTextureFsName = "Unlit/UnlitTextureFs.fs";
    constexpr const char *UnlitTextureVsName = "Unlit/UnlitTextureVs.vs";

    constexpr const char *PhongTexFs = "Lit/PhongTexFs.fs";
    constexpr const char *PhongTexVs = "Lit/PhongTexVs.vs";
    constexpr const char *PhongTexInstancedVs = "Lit/PhongTexInstancedVs.vs";
    constexpr const char *PhongSpecularTextureFs = "Lit/PhongSpecularTextureFs.fs";
    constexpr const char *DiffTex1SpecTex2Fs = "Lit/DiffuseTex1SpecTex2Fs.fs";
    constexpr const char *ShowDepthFs = "Utils/ShowDepthFs.fs";
    constexpr const char *ShowDepthVs = "Utils/ShowDepthVs.vs";
    constexpr const char *SkyboxFs = "SkyboxFs.fs";
    constexpr const char *SkyboxVs = "SkyboxVs.vs";
    constexpr const char *CubemapReflectiveFs = "Unlit/CubemapReflectiveColorFs.fs";
    constexpr const char *CubemapReflectiveVs = "Unlit/CubemapReflectiveColorVs.vs";
    constexpr const char *CubemapRefractionFs = "Unlit/CubemapRefractionColorFs.fs";
    constexpr const char *CubemapRefractionVs = "Unlit/CubemapRefractionColorVs.vs";
    
    constexpr const char *ShadowPassVs = "Utils/ShadowPassVs.vs";
    constexpr const char *ShadowPassGs = "Utils/ShadowPassGs.gs";
    constexpr const char *OmniShadowPassVs = "Utils/OmniShadowPassVs.vs";
    constexpr const char *OmniShadowPassFs = "Utils/OmniShadowPassFs.fs";
    constexpr const char *OmniShadowPassGs = "Utils/OmniShadowPassGs.gs";
    constexpr const char *EmptyFs = "Utils/EmptyFs.fs";
    
    constexpr const char *ShadowMapDrawFs = "Utils/ShowShadowMapFs.fs";

    constexpr const char *PostEffectVs = "PostEffects/PostEffectVs.vs";
    constexpr const char *GrayscalePostEffectFs = "PostEffects/PostEffectGrayscaleFs.fs";
    constexpr const char *InvertColorsPostEffectFs = "PostEffects/PostEffectInvertColorsFs.fs";
    constexpr const char *SharpColorPostEffectFs = "PostEffects/PostEffectSharpColorFs.fs";
    constexpr const char *BlurPostEffectFs = "PostEffects/PostEffectBlurFs.fs";
    constexpr const char *EdgeDetectionPostEffectFs = "PostEffects/PostEffectEdgeDetectionFs.fs";

    constexpr const char *woodenContainerJpg = "wood_container.jpg";
    constexpr const char *brickWallJpg = "brick_wall.jpg";
    constexpr const char *awesomeFacePng = "awesomeface.png";
    constexpr const char *steeledWoodPng = "steeled_wood.png";
    constexpr const char *steeledBorderPng = "steel_border.png";
    constexpr const char *grassPng = "grass.png";
    constexpr const char *windowPng = "window.png";
    
    std::array<std::string, 6> skyboxJpg =
    {
        "SkyboxBay/Bay_XPos.jpg",
        "SkyboxBay/Bay_XNeg.jpg",
        "SkyboxBay/Bay_YPos.jpg",
        "SkyboxBay/Bay_YNeg.jpg",
        "SkyboxBay/Bay_ZPos.jpg",
        "SkyboxBay/Bay_ZNeg.jpg"
    };
    
    std::vector<float> cube
    {
             // position            // normal          // uv
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f
    };
    
    std::vector<float> quadVertices
    {
             // position          // uv
         -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
         -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,
    };

    std::vector<uint32_t> quadIndices
    {
        0, 3, 1,
        1, 3, 2
    };
    
    std::unique_ptr<Feng::TextureData> woodContainerTextureData;
    std::unique_ptr<Feng::TextureData> brickWallTextureData;
    std::unique_ptr<Feng::TextureData> awesomeFaceTextureData;
    std::unique_ptr<Feng::TextureData> steeledWoodTextureData;
    std::unique_ptr<Feng::TextureData> steelBorderTextureData;
    std::unique_ptr<Feng::TextureData> grassTextureData;
    std::unique_ptr<Feng::TextureData> windowTextureData;
    std::array<std::unique_ptr<Feng::TextureData>, 6> skyboxData;

    std::unique_ptr<Feng::PostEffectDefinition> grayscaleFx;
    std::unique_ptr<Feng::PostEffectDefinition> invertColorsFx;
    std::unique_ptr<Feng::PostEffectDefinition> sharpColorFx;
    std::unique_ptr<Feng::PostEffectDefinition> blurFx;
    std::unique_ptr<Feng::PostEffectDefinition> edgeDetectionFx;
    
    std::unique_ptr<Feng::Shader> LoadShader(
                                             const std::string& vsFileName,
                                             const std::string& fsFileName,
                                             const std::string& gsFileName = "")
    {
        std::string vsFilePath = BaseShadersDir + vsFileName;
        std::string fsFilePath = BaseShadersDir + fsFileName;
        std::string gsFilePath = gsFileName.empty() ? std::string{""} : BaseShadersDir + gsFileName;
        return Feng::LoadShader(vsFilePath, fsFilePath, gsFilePath);
    }

    std::unique_ptr<Feng::TextureData> LoadTexture(std::string name, bool flip)
    {
        std::string texturePath = BaseTexturesDir + name;
        return Feng::TextureData::Load(texturePath, flip);
    }

    std::array<std::unique_ptr<Feng::TextureData>, 6> LoadCubemapTexture(std::array<std::string, 6> relativePaths, bool flip)
    {
        std::array<std::string, 6> absolutePaths;
        std::transform(
                relativePaths.begin(),
                relativePaths.end(),
                absolutePaths.begin(),
                [](const std::string& relativePath) { return BaseTexturesDir + relativePath; });

        return Feng::TextureData::LoadCubemap(absolutePaths, { flip, flip, flip, flip, flip, flip });
    }

    void LoadTextures()
    {
        using namespace Feng;
        
        woodContainerTextureData = LoadTexture(woodenContainerJpg, false);
        test::res.WoodContainerTexture = std::make_unique<Texture>(*woodContainerTextureData);
        test::res.WoodContainerTexture->SetFilters(eTextureMinFilter::LinearMipLinear, eTextureMagFilter::Linear);
        test::res.WoodContainerTexture->SetUseMipmaps(true);

        brickWallTextureData = LoadTexture(brickWallJpg, false);
        test::res.BrickWallTexture = std::make_unique<Texture>(*brickWallTextureData);

        awesomeFaceTextureData = LoadTexture(awesomeFacePng, true);
        test::res.AwesomeFaceTexture = std::make_unique<Texture>(*awesomeFaceTextureData);

        steeledWoodTextureData = LoadTexture(steeledWoodPng, true);
        test::res.SteeledWoodTexture = std::make_unique<Texture>(*steeledWoodTextureData);

        steelBorderTextureData = LoadTexture(steeledBorderPng, true);
        test::res.SteelBorderTexture = std::make_unique<Texture>(*steelBorderTextureData);

        grassTextureData = LoadTexture(grassPng, true);
        test::res.GrassTexture = std::make_unique<Texture>(*grassTextureData);
        test::res.GrassTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);

        windowTextureData = LoadTexture(windowPng, true);
        test::res.WindowTexture = std::make_unique<Texture>(*windowTextureData);
        test::res.WindowTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);

        skyboxData = LoadCubemapTexture(skyboxJpg, false);
        std::array<const TextureData*, 6> skyboxFaces;
        for(uint8_t i = 0; i < 6; ++i)
        {
            skyboxFaces[i] = skyboxData[i].get();
        }

        test::res.SkyboxTexture = std::make_unique<Texture>(skyboxFaces);
    }

    void LoadMaterials()
    {
        using namespace Feng;
        LoadTextures();

        std::unique_ptr<Shader> phongTexShader = LoadShader(PhongTexVs, PhongTexFs);
        test::res.PhongTexMaterial = std::make_unique<Material>(std::move(phongTexShader));
        test::res.PhongTexMaterial->SetTexture(ShaderParams::Texture0.data(), test::res.WoodContainerTexture.get());
        test::res.PhongTexMaterial->SetTransparent(false);

        std::unique_ptr<Shader> phongTexInstancedShader = LoadShader(PhongTexInstancedVs, PhongTexFs);
        test::res.PhongTexInstancedMaterial = std::make_unique<Material>(std::move(phongTexInstancedShader));
        test::res.PhongTexInstancedMaterial->SetTexture(ShaderParams::Texture0.data(), test::res.WoodContainerTexture.get());
        test::res.PhongTexInstancedMaterial->SetTransparent(false);

        std::unique_ptr<Shader> specularTextureShader = LoadShader(PhongTexVs, PhongSpecularTextureFs);
        test::res.SpecularTexMaterial = std::make_unique<Material>(std::move(specularTextureShader));
        test::res.SpecularTexMaterial->SetTexture(ShaderParams::Texture0.data(), test::res.BrickWallTexture.get());
        test::res.SpecularTexMaterial->SetFloat("uSpecularity", 1.0f);
        test::res.SpecularTexMaterial->SetFloat("uShininess", 64.0f);
        test::res.SpecularTexMaterial->SetTransparent(false);

        std::unique_ptr<Shader> diff1Spec2Shader = LoadShader(PhongTexVs, DiffTex1SpecTex2Fs);
        test::res.DiffTex1SpecTex2Material = std::make_unique<Material>(std::move(diff1Spec2Shader));
        test::res.DiffTex1SpecTex2Material->SetTexture(ShaderParams::Texture0.data(), test::res.SteeledWoodTexture.get());
        test::res.DiffTex1SpecTex2Material->SetTexture(ShaderParams::Texture1.data(), test::res.SteelBorderTexture.get());
        test::res.DiffTex1SpecTex2Material->SetFloat("uSpecularity", 5.f);
        test::res.DiffTex1SpecTex2Material->SetFloat("uShininess", 64.0f);
        test::res.DiffTex1SpecTex2Material->SetTransparent(false);

        test::res.ShowDepthMaterial = std::make_unique<Material>(LoadShader(ShowDepthVs, ShowDepthFs));

        test::res.GrassMaterial = std::make_unique<Material>(LoadShader(UnlitTextureVsName, UnlitTextureFsName));
        test::res.GrassMaterial->SetTexture(ShaderParams::Texture0.data(), test::res.GrassTexture.get());
        test::res.GrassMaterial->SetDrawFace(eDrawFace::Both);
        test::res.GrassMaterial->SetTransparent(true);

        test::res.WindowMaterial = std::make_unique<Material>(LoadShader(UnlitTextureVsName, UnlitTextureFsName));
        test::res.WindowMaterial->SetTexture(ShaderParams::Texture0.data(), test::res.WindowTexture.get());
        test::res.WindowMaterial->SetDrawFace(eDrawFace::Both);
        test::res.WindowMaterial->SetTransparent(true);

        test::res.SkyboxMaterial = std::make_unique<Material>(LoadShader(SkyboxVs, SkyboxFs));
        test::res.SkyboxMaterial->SetTexture(ShaderParams::Texture0.data(), test::res.SkyboxTexture.get());
        test::res.SkyboxMaterial->SetTransparent(false);
        
        test::res.CubemapReflectiveMaterial = std::make_unique<Material>(LoadShader(CubemapReflectiveVs, CubemapReflectiveFs));
        test::res.CubemapReflectiveMaterial->SetTexture(ShaderParams::Texture0.data(), test::res.SkyboxTexture.get());
        test::res.CubemapReflectiveMaterial->SetTransparent(false);
        
        test::res.CubemapRefractionMaterial = std::make_unique<Material>(LoadShader(CubemapRefractionVs, CubemapRefractionFs));
        test::res.CubemapRefractionMaterial->SetTexture(ShaderParams::Texture0.data(), test::res.SkyboxTexture.get());
        test::res.CubemapRefractionMaterial->SetTransparent(false);
        
        test::res.DirectShadowPassMaterial = std::make_unique<Material>(LoadShader(ShadowPassVs, EmptyFs, ShadowPassGs));
        test::res.OmniShadowPassMaterial = std::make_unique<Material>(LoadShader(OmniShadowPassVs, OmniShadowPassFs, OmniShadowPassGs));
        test::res.ShadowMapDrawMaterial = std::make_unique<Material>(LoadShader(PostEffectVs, ShadowMapDrawFs));
    }

    void CreatePostEffectDefinitions()
    {
        using namespace Feng;
        
        std::unique_ptr<Shader> grayscaleShader = LoadShader(PostEffectVs, GrayscalePostEffectFs);
        std::unique_ptr<Material> grayscaleFxMaterial = std::make_unique<Material>(std::move(grayscaleShader));
        grayscaleFx = std::make_unique<PostEffectDefinition>(std::move(grayscaleFxMaterial));

        std::unique_ptr<Shader> invertColorsShader = LoadShader(PostEffectVs, InvertColorsPostEffectFs);
        std::unique_ptr<Material> invertColorsFxMaterial = std::make_unique<Material>(std::move(invertColorsShader));
        invertColorsFx = std::make_unique<PostEffectDefinition>(std::move(invertColorsFxMaterial));

        std::unique_ptr<Shader> sharpColorShader = LoadShader(PostEffectVs, SharpColorPostEffectFs);
        std::unique_ptr<Material> sharpColorFxMaterial = std::make_unique<Material>(std::move(sharpColorShader));
        sharpColorFx = std::make_unique<PostEffectDefinition>(std::move(sharpColorFxMaterial));

        std::unique_ptr<Shader> blurShader = LoadShader(PostEffectVs, BlurPostEffectFs);
        std::unique_ptr<Material> blurFxMaterial = std::make_unique<Material>(std::move(blurShader));
        blurFx = std::make_unique<PostEffectDefinition>(std::move(blurFxMaterial));

        std::unique_ptr<Shader> edgeDetectionShader = LoadShader(PostEffectVs, EdgeDetectionPostEffectFs);
        std::unique_ptr<Material> edgeDetectionFxMaterial = std::make_unique<Material>(std::move(edgeDetectionShader));
        edgeDetectionFx = std::make_unique<PostEffectDefinition>(std::move(edgeDetectionFxMaterial));

        test::res.Effects.push_back(blurFx.get());
        test::res.Effects.push_back(grayscaleFx.get());
        test::res.Effects.push_back(invertColorsFx.get());
        test::res.Effects.push_back(sharpColorFx.get());
        test::res.Effects.push_back(edgeDetectionFx.get());
    }

    void LoadMeshes()
    {
        using namespace Feng;
        
        uint32_t cubeAttributesValue = eVertexAtributes::Position | eVertexAtributes::Normal | eVertexAtributes::Uv0;
        eVertexAtributes cubeAttributes = static_cast<eVertexAtributes>(cubeAttributesValue);
        test::res.CubeMesh = std::make_unique<Mesh>(cube, cubeAttributes, ePrimitiveType::Triangles);

        uint32_t quadAttributesValue = eVertexAtributes::Position | eVertexAtributes::Uv0;
        eVertexAtributes quadAttributes = static_cast<eVertexAtributes>(quadAttributesValue);
        test::res.QuadMesh = std::make_unique<Mesh>(quadVertices, quadIndices, quadAttributes,  ePrimitiveType::Triangles);
    }
}

namespace test
{
    TempRes res{};
    
    std::unique_ptr<Feng::Material> CreateFlatColorMaterial()
    {
        std::unique_ptr<Feng::Shader> flatColorShader = LoadShader(FlatColorVsName, FlatColorFsName);
        return std::make_unique<Feng::Material>(std::move(flatColorShader));
    }

    std::unique_ptr<Feng::Material> CreateGizmoMaterial(bool showDepth)
    {
        if(showDepth)
        {
            return std::make_unique<Feng::Material>(LoadShader(ShowDepthVs, ShowDepthFs));
        }

        return CreateFlatColorMaterial();
    }

    void LoadResources()
    {
        LoadMaterials();
        LoadMeshes();
        CreatePostEffectDefinitions();
    }
}
