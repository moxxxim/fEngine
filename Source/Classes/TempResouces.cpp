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

    namespace ShaderSrc
    {
        constexpr const char *FlatColorFsName = "Unlit/UnlitFlatColorFs.shader";
        constexpr const char *FlatColorVsName = "Unlit/UnlitFlatColorVs.shader";
        constexpr const char *UnlitTextureFsName = "Unlit/UnlitTextureFs.shader";
        constexpr const char *UnlitTextureVsName = "Unlit/UnlitTextureVs.shader";

        constexpr const char *PhongTexFs = "Lit/PhongTexFs.shader";
        constexpr const char *PhongTexVs = "Lit/PhongTexVs.shader";
        constexpr const char *PhongTexInstancedVs = "Lit/PhongTexInstancedVs.shader";
        constexpr const char *PhongSpecularTextureFs = "Lit/PhongSpecularTextureFs.shader";
        constexpr const char *DiffTex1SpecTex2Fs = "Lit/DiffuseTex1SpecTex2Fs.shader";
        constexpr const char *ShowDepthFs = "Utils/ShowDepthFs.shader";
        constexpr const char *ShowDepthVs = "Utils/ShowDepthVs.shader";
        constexpr const char *SkyboxFs = "SkyboxFs.shader";
        constexpr const char *SkyboxVs = "SkyboxVs.shader";
        constexpr const char *CubemapReflectiveFs = "Unlit/CubemapReflectiveColorFs.shader";
        constexpr const char *CubemapReflectiveVs = "Unlit/CubemapReflectiveColorVs.shader";
        constexpr const char *CubemapRefractionFs = "Unlit/CubemapRefractionColorFs.shader";
        constexpr const char *CubemapRefractionVs = "Unlit/CubemapRefractionColorVs.shader";
        constexpr const char *PhongTexWithNormalMapFs = "Lit/PhongTexWithNmFs.shader";
        constexpr const char *PhongTexWithNormalMapVs = "Lit/PhongTexWithNmVs.shader";
        constexpr const char *PhongParallaxFs = "Lit/PhongParallaxFs.shader";
        constexpr const char *PhongParallaxVs = "Lit/PhongParallaxVs.shader";
        
        constexpr const char *ShadowPassVs = "Utils/ShadowPassVs.shader";
        constexpr const char *ShadowPassGs = "Utils/ShadowPassGs.shader";
        constexpr const char *OmniShadowPassVs = "Utils/OmniShadowPassVs.shader";
        constexpr const char *OmniShadowPassFs = "Utils/OmniShadowPassFs.shader";
        constexpr const char *OmniShadowPassGs = "Utils/OmniShadowPassGs.shader";
        constexpr const char *EmptyFs = "Utils/EmptyFs.shader";
        
        constexpr const char *ShadowMapDrawFs = "Utils/ShowShadowMapFs.shader";

        constexpr const char *PostEffectVs = "PostEffects/PostEffectVs.shader";
        constexpr const char *GrayscalePostEffectFs = "PostEffects/PostEffectGrayscaleFs.shader";
        constexpr const char *InvertColorsPostEffectFs = "PostEffects/PostEffectInvertColorsFs.shader";
        constexpr const char *SharpColorPostEffectFs = "PostEffects/PostEffectSharpColorFs.shader";
        constexpr const char *BlurPostEffectFs = "PostEffects/PostEffectBlurFs.shader";
        constexpr const char *EdgeDetectionPostEffectFs = "PostEffects/PostEffectEdgeDetectionFs.shader";
    }

    namespace Tex
    {
        constexpr const char *WoodenContainerJpg = "wood_container.jpg";
        constexpr const char *TileWallJpg = "tile.jpg";
        constexpr const char *AwesomeFacePng = "awesomeface.png";
        constexpr const char *SteeledWoodPng = "steeled_wood.png";
        constexpr const char *SteeledBorderPng = "steel_border.png";
        constexpr const char *GrassPng = "grass.png";
        constexpr const char *WindowPng = "window.png";
        constexpr const char *BrickwallBcJpg = "Brickwall_BC.jpg";
        constexpr const char *BrickwallNmJpg = "Brickwall_NM.jpg";
        constexpr const char *Brickwall2BcJpg = "BrickWall2_BC.jpg";
        constexpr const char *Brickwall2NmJpg = "BrickWall2_NM.jpg";
        constexpr const char *Brickwall2DepthJpg = "BrickWall2_Depth.jpg";
        
        std::array<std::string, 6> SkyboxJpg =
        {
            "SkyboxBay/Bay_XPos.jpg",
            "SkyboxBay/Bay_XNeg.jpg",
            "SkyboxBay/Bay_YPos.jpg",
            "SkyboxBay/Bay_YNeg.jpg",
            "SkyboxBay/Bay_ZPos.jpg",
            "SkyboxBay/Bay_ZNeg.jpg"
        };
    }

    std::vector<float> cube
    {
             // position            // normal          // uv          // tangent            // bitangent

        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
        
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
        
        

        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        
        

        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,  0.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        
        

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
        
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  1.0f, 0.0f,
        
        

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        
        

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,  1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
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
    std::unique_ptr<Feng::TextureData> tileWallTextureData;
    std::unique_ptr<Feng::TextureData> awesomeFaceTextureData;
    std::unique_ptr<Feng::TextureData> steeledWoodTextureData;
    std::unique_ptr<Feng::TextureData> steelBorderTextureData;
    std::unique_ptr<Feng::TextureData> grassTextureData;
    std::unique_ptr<Feng::TextureData> windowTextureData;
    std::unique_ptr<Feng::TextureData> brickwallBcTextureData;
    std::unique_ptr<Feng::TextureData> brickwallNmTextureData;
    std::unique_ptr<Feng::TextureData> brickwall2BcTextureData;
    std::unique_ptr<Feng::TextureData> brickwall2NmTextureData;
    std::unique_ptr<Feng::TextureData> brickwall2DepthTextureData;
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
        
        woodContainerTextureData = LoadTexture(Tex::WoodenContainerJpg, false);
        test::res.WoodContainerTexture = std::make_unique<Texture>(*woodContainerTextureData);
        test::res.WoodContainerTexture->SetFilters(eTextureMinFilter::LinearMipLinear, eTextureMagFilter::Linear);
        test::res.WoodContainerTexture->SetUseMipmaps(true);

        tileWallTextureData = LoadTexture(Tex::TileWallJpg, false);
        test::res.TileWallTexture = std::make_unique<Texture>(*tileWallTextureData);

        awesomeFaceTextureData = LoadTexture(Tex::AwesomeFacePng, true);
        test::res.AwesomeFaceTexture = std::make_unique<Texture>(*awesomeFaceTextureData);

        steeledWoodTextureData = LoadTexture(Tex::SteeledWoodPng, true);
        test::res.SteeledWoodTexture = std::make_unique<Texture>(*steeledWoodTextureData);

        steelBorderTextureData = LoadTexture(Tex::SteeledBorderPng, true);
        test::res.SteelBorderTexture = std::make_unique<Texture>(*steelBorderTextureData);

        grassTextureData = LoadTexture(Tex::GrassPng, true);
        test::res.GrassTexture = std::make_unique<Texture>(*grassTextureData);
        test::res.GrassTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);

        windowTextureData = LoadTexture(Tex::WindowPng, true);
        test::res.WindowTexture = std::make_unique<Texture>(*windowTextureData);
        test::res.WindowTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);
        
        brickwallBcTextureData = LoadTexture(Tex::BrickwallBcJpg, false);
        test::res.BrickwallBcTexture = std::make_unique<Texture>(*brickwallBcTextureData);
        test::res.BrickwallBcTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);
        
        brickwallNmTextureData = LoadTexture(Tex::BrickwallNmJpg, false);
        test::res.BrickwallNmTexture = std::make_unique<Texture>(*brickwallNmTextureData);
        test::res.BrickwallNmTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);

        brickwall2BcTextureData = LoadTexture(Tex::Brickwall2BcJpg, false);
        test::res.Brickwall2BcTexture = std::make_unique<Texture>(*brickwall2BcTextureData);
        test::res.Brickwall2BcTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);
        
        brickwall2NmTextureData = LoadTexture(Tex::Brickwall2NmJpg, false);
        test::res.Brickwall2NmTexture = std::make_unique<Texture>(*brickwall2NmTextureData);
        test::res.Brickwall2NmTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);
        
        brickwall2DepthTextureData = LoadTexture(Tex::Brickwall2DepthJpg, false);
        test::res.Brickwall2DepthTexture = std::make_unique<Texture>(*brickwall2DepthTextureData);
        test::res.Brickwall2DepthTexture->SetWrapping(eTextureWrapping::ClampToEdge, Feng::eTextureWrapping::ClampToEdge);

        skyboxData = LoadCubemapTexture(Tex::SkyboxJpg, false);
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

        std::unique_ptr<Shader> phongTexShader = LoadShader(ShaderSrc::PhongTexVs, ShaderSrc::PhongTexFs);
        test::res.WoodContainerMaterial = std::make_unique<Material>(std::move(phongTexShader));
        test::res.WoodContainerMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.WoodContainerTexture.get());
        test::res.WoodContainerMaterial->SetTransparent(false);
        
        std::unique_ptr<Shader> phongTexShader2 = LoadShader(ShaderSrc::PhongTexVs, ShaderSrc::PhongTexFs);
        test::res.BrickWallMaterial = std::make_unique<Material>(std::move(phongTexShader2));
        test::res.BrickWallMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.BrickwallBcTexture.get());

        std::unique_ptr<Shader> phongTexInstancedShader = LoadShader(ShaderSrc::PhongTexInstancedVs, ShaderSrc::PhongTexFs);
        test::res.WoodContainerInstancedMaterial = std::make_unique<Material>(std::move(phongTexInstancedShader));
        test::res.WoodContainerInstancedMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.WoodContainerTexture.get());
        test::res.WoodContainerInstancedMaterial->SetTransparent(false);

        std::unique_ptr<Shader> specularTextureShader = LoadShader(ShaderSrc::PhongTexVs, ShaderSrc::PhongSpecularTextureFs);
        test::res.TileWallMaterial = std::make_unique<Material>(std::move(specularTextureShader));
        test::res.TileWallMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.TileWallTexture.get());
        test::res.TileWallMaterial->Bindings().SetFloat("uSpecularity", 1.0f);
        test::res.TileWallMaterial->Bindings().SetFloat("uShininess", 64.0f);
        test::res.TileWallMaterial->SetTransparent(false);

        std::unique_ptr<Shader> phongTexWithNmShader = LoadShader(ShaderSrc::PhongTexWithNormalMapVs,
                                                                  ShaderSrc::PhongTexWithNormalMapFs);
        test::res.BrickWallWithNmMaterial = std::make_unique<Material>(std::move(phongTexWithNmShader));
        test::res.BrickWallWithNmMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.BrickwallBcTexture.get());
        test::res.BrickWallWithNmMaterial->Bindings().SetTexture(ShaderParams::NormalMap0.data(), test::res.BrickwallNmTexture.get());
        test::res.BrickWallWithNmMaterial->Bindings().SetFloat("uSpecularity", 1.0f);
        test::res.BrickWallWithNmMaterial->Bindings().SetFloat("uShininess", 64.0f);
        
        
        std::unique_ptr<Shader> phongParallaxShader = LoadShader(ShaderSrc::PhongParallaxVs,
                                                                 ShaderSrc::PhongParallaxFs);
        test::res.BrickWallParallaxMaterial = std::make_unique<Material>(std::move(phongParallaxShader));
        test::res.BrickWallParallaxMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.Brickwall2BcTexture.get());
        test::res.BrickWallParallaxMaterial->Bindings().SetTexture(ShaderParams::NormalMap0.data(), test::res.Brickwall2NmTexture.get());
        test::res.BrickWallParallaxMaterial->Bindings().SetTexture(ShaderParams::DepthMap.data(), test::res.Brickwall2DepthTexture.get());
        test::res.BrickWallParallaxMaterial->Bindings().SetFloat("uSpecularity", 1.0f);
        test::res.BrickWallParallaxMaterial->Bindings().SetFloat("uShininess", 64.0f);
        test::res.BrickWallParallaxMaterial->Bindings().SetFloat("uParallaxStrength", 0.05f);

        std::unique_ptr<Shader> diff1Spec2Shader = LoadShader(ShaderSrc::PhongTexVs, ShaderSrc::DiffTex1SpecTex2Fs);
        test::res.DiffTex1SpecTex2Material = std::make_unique<Material>(std::move(diff1Spec2Shader));
        test::res.DiffTex1SpecTex2Material->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.SteeledWoodTexture.get());
        test::res.DiffTex1SpecTex2Material->Bindings().SetTexture(ShaderParams::Texture1.data(), test::res.SteelBorderTexture.get());
        test::res.DiffTex1SpecTex2Material->Bindings().SetFloat("uSpecularity", 5.f);
        test::res.DiffTex1SpecTex2Material->Bindings().SetFloat("uShininess", 64.0f);
        test::res.DiffTex1SpecTex2Material->SetTransparent(false);

        test::res.ShowDepthMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::ShowDepthVs, ShaderSrc::ShowDepthFs));

        test::res.GrassMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::UnlitTextureVsName, ShaderSrc::UnlitTextureFsName));
        test::res.GrassMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.GrassTexture.get());
        test::res.GrassMaterial->SetDrawFace(eDrawFace::Both);
        test::res.GrassMaterial->SetTransparent(true);

        test::res.WindowMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::UnlitTextureVsName, ShaderSrc::UnlitTextureFsName));
        test::res.WindowMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.WindowTexture.get());
        test::res.WindowMaterial->SetDrawFace(eDrawFace::Both);
        test::res.WindowMaterial->SetTransparent(true);

        test::res.SkyboxMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::SkyboxVs, ShaderSrc::SkyboxFs));
        test::res.SkyboxMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.SkyboxTexture.get());
        test::res.SkyboxMaterial->SetTransparent(false);
        
        test::res.CubemapReflectiveMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::CubemapReflectiveVs, ShaderSrc::CubemapReflectiveFs));
        test::res.CubemapReflectiveMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.SkyboxTexture.get());
        test::res.CubemapReflectiveMaterial->SetTransparent(false);
        
        test::res.CubemapRefractionMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::CubemapRefractionVs, ShaderSrc::CubemapRefractionFs));
        test::res.CubemapRefractionMaterial->Bindings().SetTexture(ShaderParams::Texture0.data(), test::res.SkyboxTexture.get());
        test::res.CubemapRefractionMaterial->SetTransparent(false);
        
        test::res.DirectShadowPassMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::ShadowPassVs, ShaderSrc::EmptyFs, ShaderSrc::ShadowPassGs));
        test::res.OmniShadowPassMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::OmniShadowPassVs, ShaderSrc::OmniShadowPassFs, ShaderSrc::OmniShadowPassGs));
        test::res.ShadowMapDrawMaterial = std::make_unique<Material>(LoadShader(ShaderSrc::PostEffectVs, ShaderSrc::ShadowMapDrawFs));
    }

    void CreatePostEffectDefinitions()
    {
        using namespace Feng;
        
        std::unique_ptr<Shader> grayscaleShader = LoadShader(ShaderSrc::PostEffectVs, ShaderSrc::GrayscalePostEffectFs);
        std::unique_ptr<Material> grayscaleFxMaterial = std::make_unique<Material>(std::move(grayscaleShader));
        grayscaleFx = std::make_unique<PostEffectDefinition>(std::move(grayscaleFxMaterial));

        std::unique_ptr<Shader> invertColorsShader = LoadShader(ShaderSrc::PostEffectVs, ShaderSrc::InvertColorsPostEffectFs);
        std::unique_ptr<Material> invertColorsFxMaterial = std::make_unique<Material>(std::move(invertColorsShader));
        invertColorsFx = std::make_unique<PostEffectDefinition>(std::move(invertColorsFxMaterial));

        std::unique_ptr<Shader> sharpColorShader = LoadShader(ShaderSrc::PostEffectVs, ShaderSrc::SharpColorPostEffectFs);
        std::unique_ptr<Material> sharpColorFxMaterial = std::make_unique<Material>(std::move(sharpColorShader));
        sharpColorFx = std::make_unique<PostEffectDefinition>(std::move(sharpColorFxMaterial));

        std::unique_ptr<Shader> blurShader = LoadShader(ShaderSrc::PostEffectVs, ShaderSrc::BlurPostEffectFs);
        std::unique_ptr<Material> blurFxMaterial = std::make_unique<Material>(std::move(blurShader));
        blurFx = std::make_unique<PostEffectDefinition>(std::move(blurFxMaterial));

        std::unique_ptr<Shader> edgeDetectionShader = LoadShader(ShaderSrc::PostEffectVs, ShaderSrc::EdgeDetectionPostEffectFs);
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
        
        uint32_t cubeAttributesValue = Feng::eVertexAtributes::Position
                                     | Feng::eVertexAtributes::Normal
                                     | Feng::eVertexAtributes::Tangent
                                     | Feng::eVertexAtributes::Bitangent
                                     | Feng::eVertexAtributes::Uv0;

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
        std::unique_ptr<Feng::Shader> flatColorShader = LoadShader(ShaderSrc::FlatColorVsName, ShaderSrc::FlatColorFsName);
        return std::make_unique<Feng::Material>(std::move(flatColorShader));
    }

    std::unique_ptr<Feng::Material> CreateGizmoMaterial(bool showDepth)
    {
        if(showDepth)
        {
            return std::make_unique<Feng::Material>(LoadShader(ShaderSrc::ShowDepthVs, ShaderSrc::ShowDepthFs));
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
