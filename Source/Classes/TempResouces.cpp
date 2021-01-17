#include <Classes/TempResouces.h>

#include <Feng/Render/PostEffects/PostEffectDefinition.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/ResourcesManager/TextureData.h>
#include <Feng/Utils/Render/ShaderParams.h>

#include <array>
#include <cstdint>
#include <string>

std::string BaseResourcesDir = "../../../Resources/";
std::string BaseTexturesDir = BaseResourcesDir + "Textures/";
std::string BaseShadersDir = BaseResourcesDir + "Shaders/";

constexpr const char *FlatColorFsName = "Unlit/UnlitFlatColorFs.fs";
constexpr const char *FlatColorVsName = "Unlit/UnlitFlatColorVs.vs";
constexpr const char *UnlitTextureFsName = "Unlit/UnlitTextureFs.fs";
constexpr const char *UnlitTextureVsName = "Unlit/UnlitTextureVs.vs";

constexpr const char *DiffuseTextureFsName = "DiffuseTextureFs.fs";
constexpr const char *DiffuseTextureVsName = "DiffuseTextureVs.vs";
constexpr const char *SpecularTextureFsName = "SpecularTextureFs.fs";
constexpr const char *SpecularTextureVsName = "SpecularTextureVs.vs";
constexpr const char *DiffTex1SpecTex2FsName = "DiffuseTex1SpecTex2Fs.fs";
constexpr const char *DiffTex1SpecTex2VsName = "DiffuseTex1SpecTex2Vs.vs";
constexpr const char *ShowDepthFsName = "Utils/ShowDepthFs.fs";
constexpr const char *ShowDepthVsName = "Utils/ShowDepthVs.vs";
constexpr const char *SkyboxFsName = "SkyboxFs.fs";
constexpr const char *SkyboxVsName = "SkyboxVs.vs";
constexpr const char *CubemapReflectiveFsName = "Unlit/CubemapReflectiveColorFs.fs";
constexpr const char *CubemapReflectiveVsName = "Unlit/CubemapReflectiveColorVs.vs";

constexpr const char *PostEffectVsName = "PostEffects/PostEffectVs.vs";
constexpr const char *GrayscalePostEffectFsName = "PostEffects/PostEffectGrayscaleFs.fs";
constexpr const char *InvertColorsPostEffectFsName = "PostEffects/PostEffectInvertColorsFs.fs";
constexpr const char *SharpColorPostEffectFsName = "PostEffects/PostEffectSharpColorFs.fs";
constexpr const char *BlurPostEffectFsName = "PostEffects/PostEffectBlurFs.fs";
constexpr const char *EdgeDetectionPostEffectFsName = "PostEffects/PostEffectEdgeDetectionFs.fs";

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

std::unique_ptr<feng::TextureData> woodContainerTextureData;
std::unique_ptr<feng::TextureData> brickWallTextureData;
std::unique_ptr<feng::TextureData> awesomeFaceTextureData;
std::unique_ptr<feng::TextureData> steeledWoodTextureData;
std::unique_ptr<feng::TextureData> steelBorderTextureData;
std::unique_ptr<feng::TextureData> grassTextureData;
std::unique_ptr<feng::TextureData> windowTextureData;
std::array<std::unique_ptr<feng::TextureData>, 6> skyboxData;

std::unique_ptr<feng::PostEffectDefinition> grayscaleEffect;
std::unique_ptr<feng::PostEffectDefinition> invertColorsEffect;
std::unique_ptr<feng::PostEffectDefinition> sharpColorEffect;
std::unique_ptr<feng::PostEffectDefinition> blurEffect;
std::unique_ptr<feng::PostEffectDefinition> edgeDetectionEffect;

TempRes res{};

std::unique_ptr<feng::Shader> LoadShader(const std::string& vsFileName, const std::string& fsFileName)
{
    std::string vsFilePath = BaseShadersDir + vsFileName;
    std::string fsFilePath = BaseShadersDir + fsFileName;
    return feng::LoadShader(vsFilePath, fsFilePath);
}

std::unique_ptr<feng::TextureData> LoadTexture(std::string name, bool flip)
{
    std::string texturePath = BaseTexturesDir + name;
    return feng::TextureData::Load(texturePath, flip);
}

std::array<std::unique_ptr<feng::TextureData>, 6> LoadCubemapTexture(std::array<std::string, 6> relativePaths, bool flip)
{
    std::array<std::string, 6> absolutePaths;
    std::transform(
            relativePaths.begin(),
            relativePaths.end(),
            absolutePaths.begin(),
            [](const std::string& relativePath) { return BaseTexturesDir + relativePath; });

    return feng::TextureData::LoadCubemap(absolutePaths, { flip, flip, flip, flip, flip, flip });
}

void LoadTextures()
{
    woodContainerTextureData = LoadTexture(woodenContainerJpg, false);
    res.WoodContainerTexture = std::make_unique<feng::Texture>(*woodContainerTextureData);
    res.WoodContainerTexture->SetFilters(feng::eTextureMinFilter::Linear, feng::eTextureMagFilter::Linear);

    brickWallTextureData = LoadTexture(brickWallJpg, false);
    res.BrickWallTexture = std::make_unique<feng::Texture>(*brickWallTextureData);

    awesomeFaceTextureData = LoadTexture(awesomeFacePng, true);
    res.AwesomeFaceTexture = std::make_unique<feng::Texture>(*awesomeFaceTextureData);

    steeledWoodTextureData = LoadTexture(steeledWoodPng, true);
    res.SteeledWoodTexture = std::make_unique<feng::Texture>(*steeledWoodTextureData);

    steelBorderTextureData = LoadTexture(steeledBorderPng, true);
    res.SteelBorderTexture = std::make_unique<feng::Texture>(*steelBorderTextureData);

    grassTextureData = LoadTexture(grassPng, true);
    res.GrassTexture = std::make_unique<feng::Texture>(*grassTextureData);
    res.GrassTexture->SetWrapping(feng::eTextureWrapping::ClampToEdge, feng::eTextureWrapping::ClampToEdge);

    windowTextureData = LoadTexture(windowPng, true);
    res.WindowTexture = std::make_unique<feng::Texture>(*windowTextureData);
    res.WindowTexture->SetWrapping(feng::eTextureWrapping::ClampToEdge, feng::eTextureWrapping::ClampToEdge);

    skyboxData = LoadCubemapTexture(skyboxJpg, false);
    std::array<const feng::TextureData*, 6> skyboxFaces;
    for(uint8_t i = 0; i < 6; ++i)
    {
        skyboxFaces[i] = skyboxData[i].get();
    }

    res.SkyboxTexture = std::make_unique<feng::Texture>(skyboxFaces);
}

void LoadMaterials()
{
    LoadTextures();

    std::unique_ptr<feng::Shader> diffuseTextureShader = LoadShader(DiffuseTextureVsName, DiffuseTextureFsName);
    res.DiffuseTexMaterial = std::make_unique<feng::Material>(std::move(diffuseTextureShader));
    res.DiffuseTexMaterial->SetTexture(feng::ShaderParams::Texture0.data(), res.WoodContainerTexture.get());

    std::unique_ptr<feng::Shader> specularTextureShader = LoadShader(SpecularTextureVsName, SpecularTextureFsName);
    res.SpecularTexMaterial = std::make_unique<feng::Material>(std::move(specularTextureShader));
    res.SpecularTexMaterial->SetTexture(feng::ShaderParams::Texture0.data(), res.BrickWallTexture.get());
    res.SpecularTexMaterial->SetFloat("uSpecularity", 1.0f);
    res.SpecularTexMaterial->SetFloat("uShininess", 32.0f);

    std::unique_ptr<feng::Shader> diff1Spec2Shader = LoadShader(DiffTex1SpecTex2VsName, DiffTex1SpecTex2FsName);
    res.DiffTex1SpecTex2Material = std::make_unique<feng::Material>(std::move(diff1Spec2Shader));
    res.DiffTex1SpecTex2Material->SetTexture(feng::ShaderParams::Texture0.data(), res.SteeledWoodTexture.get());
    res.DiffTex1SpecTex2Material->SetTexture(feng::ShaderParams::Texture1.data(), res.SteelBorderTexture.get());
    res.DiffTex1SpecTex2Material->SetFloat("uSpecularity", 5.f);
    res.DiffTex1SpecTex2Material->SetFloat("uShininess", 32.0f);

    res.ShowDepthMaterial = std::make_unique<feng::Material>(LoadShader(ShowDepthVsName, ShowDepthFsName));

    res.GrassMaterial = std::make_unique<feng::Material>(LoadShader(UnlitTextureVsName, UnlitTextureFsName));
    res.GrassMaterial->SetTexture(feng::ShaderParams::Texture0.data(), res.GrassTexture.get());

    res.WindowMaterial = std::make_unique<feng::Material>(LoadShader(UnlitTextureVsName, UnlitTextureFsName));
    res.WindowMaterial->SetTexture(feng::ShaderParams::Texture0.data(), res.WindowTexture.get());

    res.SkyboxMaterial = std::make_unique<feng::Material>(LoadShader(SkyboxVsName, SkyboxFsName));
    res.SkyboxMaterial->SetTexture(feng::ShaderParams::Texture0.data(), res.SkyboxTexture.get());

    res.CubemapReflectiveMaterial = std::make_unique<feng::Material>(LoadShader(CubemapReflectiveVsName, CubemapReflectiveFsName));
    res.CubemapReflectiveMaterial->SetTexture(feng::ShaderParams::Texture0.data(), res.SkyboxTexture.get());
}

void CreatePostEffectDefinitions()
{
    std::unique_ptr<feng::Shader> grayscaleShader = LoadShader(PostEffectVsName, GrayscalePostEffectFsName);
    std::unique_ptr<feng::Material> grayscaleEffectMaterial = std::make_unique<feng::Material>(std::move(grayscaleShader));
    grayscaleEffect = std::make_unique<feng::PostEffectDefinition>(std::move(grayscaleEffectMaterial));

    std::unique_ptr<feng::Shader> invertColorsShader = LoadShader(PostEffectVsName, InvertColorsPostEffectFsName);
    std::unique_ptr<feng::Material> invertColorsEffectMaterial = std::make_unique<feng::Material>(std::move(invertColorsShader));
    invertColorsEffect = std::make_unique<feng::PostEffectDefinition>(std::move(invertColorsEffectMaterial));

    std::unique_ptr<feng::Shader> sharpColorShader = LoadShader(PostEffectVsName, SharpColorPostEffectFsName);
    std::unique_ptr<feng::Material> sharpColorEffectMaterial = std::make_unique<feng::Material>(std::move(sharpColorShader));
    sharpColorEffect = std::make_unique<feng::PostEffectDefinition>(std::move(sharpColorEffectMaterial));

    std::unique_ptr<feng::Shader> blurShader = LoadShader(PostEffectVsName, BlurPostEffectFsName);
    std::unique_ptr<feng::Material> blurEffectMaterial = std::make_unique<feng::Material>(std::move(blurShader));
    blurEffect = std::make_unique<feng::PostEffectDefinition>(std::move(blurEffectMaterial));

    std::unique_ptr<feng::Shader> edgeDetectionShader = LoadShader(PostEffectVsName, EdgeDetectionPostEffectFsName);
    std::unique_ptr<feng::Material> edgeDetectionEffectMaterial = std::make_unique<feng::Material>(std::move(edgeDetectionShader));
    edgeDetectionEffect = std::make_unique<feng::PostEffectDefinition>(std::move(edgeDetectionEffectMaterial));

    res.Effects.push_back(blurEffect.get());
    res.Effects.push_back(grayscaleEffect.get());
    res.Effects.push_back(invertColorsEffect.get());
    res.Effects.push_back(sharpColorEffect.get());
    res.Effects.push_back(edgeDetectionEffect.get());
}

void LoadMeshes()
{
    uint32_t cubeAttributesValue = feng::eVertexAtributes::Position | feng::eVertexAtributes::Normal | feng::eVertexAtributes::Uv0;
    feng::eVertexAtributes cubeAttributes = static_cast<feng::eVertexAtributes>(cubeAttributesValue);
    res.CubeMesh = std::make_unique<feng::Mesh>(cube, cubeAttributes, feng::ePrimitiveType::Triangles);

    uint32_t quadAttributesValue = feng::eVertexAtributes::Position | feng::eVertexAtributes::Uv0;
    feng::eVertexAtributes quadAttributes = static_cast<feng::eVertexAtributes>(quadAttributesValue);
    res.QuadMesh = std::make_unique<feng::Mesh>(quadVertices, quadIndices, quadAttributes,  feng::ePrimitiveType::Triangles);
}

std::unique_ptr<feng::Material> CreateFlatColorMaterial()
{
    std::unique_ptr<feng::Shader> flatColorShader = LoadShader(FlatColorVsName, FlatColorFsName);
    return std::make_unique<feng::Material>(std::move(flatColorShader));
}

std::unique_ptr<feng::Material> CreateGizmoMaterial(bool showDepth)
{
    if(showDepth)
    {
        return std::make_unique<feng::Material>(LoadShader(ShowDepthVsName, ShowDepthFsName));
    }

    return CreateFlatColorMaterial();
}

void LoadResources()
{
    LoadMaterials();
    LoadMeshes();
    CreatePostEffectDefinitions();
}
