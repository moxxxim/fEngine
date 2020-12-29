#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <Feng/App/Globals.h>
#include <Feng/Math/MathUtils.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/Math/Vector3.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/Utils/Render/ShaderParams.h>
#include <Feng/ResourcesManager/TextureData.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Mesh.h>
#include <Feng/ResourcesManager/Texture.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Light.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/MeshRenderer.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/ScenesManager/RenderProperties.h>
#include <Feng/Render/PostEffects/RenderPostProcessing.h>
#include <Feng/Render/PostEffects/PostEffectDefinition.h>
#include <Feng/Utils/Debug.h>

#include <cmath>
#include <sstream>
#include <array>

namespace SRes
{
    std::string BaseResourcesDir = "../../../Resources/";
    std::string BaseTexturesDir = BaseResourcesDir + "Textures/";
    std::string BaseShadersDir = BaseResourcesDir + "Shaders/";

    const char *FlatColorFsName = "Unlit/UnlitFlatColorFs.fs";
    const char *FlatColorVsName = "Unlit/UnlitFlatColorVs.vs";
    const char *UnlitTextureFsName = "Unlit/UnlitTextureFs.fs";
    const char *UnlitTextureVsName = "Unlit/UnlitTextureVs.vs";

    const char *DiffuseTextureFsName = "DiffuseTextureFs.fs";
    const char *DiffuseTextureVsName = "DiffuseTextureVs.vs";
    const char *SpecularTextureFsName = "SpecularTextureFs.fs";
    const char *SpecularTextureVsName = "SpecularTextureVs.vs";
    const char *diffTex1SpecTex2FsName = "DiffuseTex1SpecTex2Fs.fs";
    const char *diffTex1SpecTex2VsName = "DiffuseTex1SpecTex2Vs.vs";
    const char *showDepthFsName = "Utils/ShowDepthFs.fs";
    const char *showDepthVsName = "Utils/ShowDepthVs.vs";

    const char *postEffectVsName = "Post Effects/PostEffectVs.vs";
    const char *grayscalePostEffectFsName = "Post Effects/PostEffectGrayscaleFs.fs";

    const char *woodenContainerJpg = "wood_container.jpg";
    const char *brickWallJpg = "brick_wall.jpg";
    const char *awesomeFacePng = "awesomeface.png";
    const char *steeledWoodPng = "steeled_wood.png";
    const char *steeledBorderPng = "steel_border.png";
    const char *grassPng = "grass.png";
    const char *windowPng = "window.png";

    std::vector<float> cube
    {
             // position            // normal          // uv
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 1.0f,

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

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f
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
    std::unique_ptr<feng::Texture> woodContainerTexture;

    std::unique_ptr<feng::TextureData> brickWallTextureData;
    std::unique_ptr<feng::Texture> brickWallTexture;

    std::unique_ptr<feng::TextureData> awesomeFaceTextureData;
    std::unique_ptr<feng::Texture> awesomeFaceTexture;

    std::unique_ptr<feng::TextureData> steeledWoodTextureData;
    std::unique_ptr<feng::Texture> steeledWoodTexture;

    std::unique_ptr<feng::TextureData> steelBorderTextureData;
    std::unique_ptr<feng::Texture> steelBorderTexture;

    std::unique_ptr<feng::TextureData> grassTextureData;
    std::unique_ptr<feng::Texture> grassTexture;

    std::unique_ptr<feng::TextureData> windowTextureData;
    std::unique_ptr<feng::Texture> windowTexture;

    std::unique_ptr<feng::Material> diffuseTexMaterial;
    std::unique_ptr<feng::Material> specularTexMaterial;
    std::unique_ptr<feng::Material> diffTex1SpecTex2Material;
    std::unique_ptr<feng::Material> showDepthMaterial;
    std::unique_ptr<feng::Material> grassMaterial;
    std::unique_ptr<feng::Material> windowMaterial;

    std::unique_ptr<feng::Mesh> cubeMesh;
    std::unique_ptr<feng::Mesh> quadMesh;

    std::unique_ptr<feng::Shader> LoadShader(const std::string& vsFileName, const std::string& fsFileName)
    {
        std::string vsFilePath = BaseShadersDir + vsFileName;
        std::string fsFilePath = BaseShadersDir + fsFileName;
        return feng::LoadShader(vsFilePath, fsFilePath);
    }

    std::unique_ptr<feng::TextureData> LoadTexture(std::string name, bool flip)
    {
        std::string texturePath = SRes::BaseTexturesDir + name;
        return feng::TextureData::Load(texturePath, flip);
    }

    void LoadTextures()
    {
        woodContainerTextureData = LoadTexture(woodenContainerJpg, false);
        woodContainerTexture = std::make_unique<feng::Texture>(*woodContainerTextureData);
        woodContainerTexture->SetFilters(feng::eTextureMinFilter::Linear, feng::eTextureMagFilter::Linear);

        brickWallTextureData = LoadTexture(brickWallJpg, false);
        brickWallTexture = std::make_unique<feng::Texture>(*brickWallTextureData);

        awesomeFaceTextureData = LoadTexture(awesomeFacePng, true);
        awesomeFaceTexture = std::make_unique<feng::Texture>(*awesomeFaceTextureData);

        steeledWoodTextureData = LoadTexture(steeledWoodPng, true);
        steeledWoodTexture = std::make_unique<feng::Texture>(*steeledWoodTextureData);

        steelBorderTextureData = LoadTexture(steeledBorderPng, true);
        steelBorderTexture = std::make_unique<feng::Texture>(*steelBorderTextureData);

        grassTextureData = LoadTexture(grassPng, true);
        grassTexture = std::make_unique<feng::Texture>(*grassTextureData);
        grassTexture->SetWrapping(feng::eTextureWrapping::ClampToEdge, feng::eTextureWrapping::ClampToEdge);

        windowTextureData = LoadTexture(windowPng, true);
        windowTexture = std::make_unique<feng::Texture>(*windowTextureData);
        windowTexture->SetWrapping(feng::eTextureWrapping::ClampToEdge, feng::eTextureWrapping::ClampToEdge);
    }

    std::unique_ptr<feng::Material> CreateFlatColorMaterial()
    {
        std::unique_ptr<feng::Shader> flatColorShader = SRes::LoadShader(SRes::FlatColorVsName, SRes::FlatColorFsName);
        return std::make_unique<feng::Material>(std::move(flatColorShader));
    }

    void LoadMaterials()
    {
        LoadTextures();

        std::unique_ptr<feng::Shader> diffuseTextureShader = LoadShader(DiffuseTextureVsName, DiffuseTextureFsName);
        diffuseTexMaterial = std::make_unique<feng::Material>(std::move(diffuseTextureShader));
        diffuseTexMaterial->SetTexture(feng::ShaderParams::Texture0.data(), woodContainerTexture.get());

        std::unique_ptr<feng::Shader> specularTextureShader = LoadShader(SpecularTextureVsName, SpecularTextureFsName);
        specularTexMaterial = std::make_unique<feng::Material>(std::move(specularTextureShader));
        specularTexMaterial->SetTexture(feng::ShaderParams::Texture0.data(), brickWallTexture.get());
        specularTexMaterial->SetFloat("uSpecularity", 1.0f);
        specularTexMaterial->SetFloat("uShininess", 32.0f);

        std::unique_ptr<feng::Shader> diff1Spec2Shader = LoadShader(diffTex1SpecTex2VsName, diffTex1SpecTex2FsName);
        diffTex1SpecTex2Material = std::make_unique<feng::Material>(std::move(diff1Spec2Shader));
        diffTex1SpecTex2Material->SetTexture(feng::ShaderParams::Texture0.data(), steeledWoodTexture.get());
        diffTex1SpecTex2Material->SetTexture(feng::ShaderParams::Texture1.data(), steelBorderTexture.get());
        diffTex1SpecTex2Material->SetFloat("uSpecularity", 5.f);
        diffTex1SpecTex2Material->SetFloat("uShininess", 32.0f);

        showDepthMaterial = std::make_unique<feng::Material>(LoadShader(showDepthVsName, showDepthFsName));

        grassMaterial = std::make_unique<feng::Material>(LoadShader(UnlitTextureVsName, UnlitTextureFsName));
        grassMaterial->SetTexture(feng::ShaderParams::Texture0.data(), grassTexture.get());

        windowMaterial = std::make_unique<feng::Material>(LoadShader(UnlitTextureVsName, UnlitTextureFsName));
        windowMaterial->SetTexture(feng::ShaderParams::Texture0.data(), windowTexture.get());
    }

    void LoadMeshes()
    {
        uint32_t cubeAttributesValue = feng::eVertexAtributes::Position | feng::eVertexAtributes::Normal | feng::eVertexAtributes::Uv0;
        feng::eVertexAtributes cubeAttributes = static_cast<feng::eVertexAtributes>(cubeAttributesValue);
        cubeMesh = std::make_unique<feng::Mesh>(cube, cubeAttributes, feng::ePrimitiveType::Triangles);

        uint32_t quadAttributesValue = feng::eVertexAtributes::Position | feng::eVertexAtributes::Uv0;
        feng::eVertexAtributes quadAttributes = static_cast<feng::eVertexAtributes>(quadAttributesValue);
        quadMesh = std::make_unique<feng::Mesh>(quadVertices, quadIndices, quadAttributes,  feng::ePrimitiveType::Triangles);
    }

    void LoadResources()
    {
        LoadMaterials();
        LoadMeshes();
    }
}

namespace SApp
{
    constexpr uint32_t InitialWidth = 800;
    constexpr uint32_t InitialHeight = 600;

    float deltaTime = 0.0f;
    float time = 0.0f;

    void UpdateTime()
    {
        float currentFrame = glfwGetTime();
        SApp::deltaTime = currentFrame - SApp::time;
        SApp::time = currentFrame;
    }
}

namespace SCamController
{
    float lastX = 0;
    float lastY = 0;
    float Zoom = 45;
    const float cameraSpeed = 3.f;
    float camPitch = 0.f;
    float camYaw = 0.f;
}

namespace SObjects
{
    std::unique_ptr<feng::Entity> camEntity;
    std::unique_ptr<feng::Entity> directLightEntity;
    std::unique_ptr<feng::Entity> pointLightEntity;
    std::unique_ptr<feng::Entity> spotLightEntity;
    std::map<feng::Light *, std::unique_ptr<feng::Material>> lightMaterials;

    std::array<feng::Vector3, 10> cubePositions = {
        feng::Vector3(-2.0f, 0.0f, 0.0f),
        feng::Vector3( 0.0f,  0.0f,  0.0f),
        feng::Vector3( 2.0f,  0.0f, 0.0f),
        feng::Vector3(-2.0f, 2.0f, 0.0f),
        feng::Vector3(0.0f, 2.0f, 0.0f),
        feng::Vector3(2.0f, 2.0f, 0.0f),
        feng::Vector3( -2.0f, 1.0f, -4.0f),
        feng::Vector3( 0.0f, 1.0f, -4.0f),
        feng::Vector3( 2.0f, 1.0f, -4.0f),
        feng::Vector3( 0.0f, -3.0f, -2.0f),
    };

    std::array<feng::Vector3, 10> vegetationPositions
    {
        feng::Vector3(10.0f, -5.4f, 10.0f),
        feng::Vector3(10.0f, -5.4f, -10.0f),
        feng::Vector3(-10.0f, -5.4f, -10.0f),
        feng::Vector3(-10.0f, -5.4f, 10.0f),
        feng::Vector3(10.0f, -5.4f, 5.0f),
        feng::Vector3(5.0f, -5.4f, 10.0f),
        feng::Vector3(5.0f, -5.4f, 5.0f),
        feng::Vector3(10.0f, -5.4f, 1.0f),
        feng::Vector3(5.0f, -5.4f, 1.0f),
        feng::Vector3(1.0f, -5.4f, 10.0f),
    };

    std::array<feng::Vector3, 8> windowPositions
    {
        feng::Vector3(10.0f, -5.4f, 20.0f),
        feng::Vector3(20.0f, -5.4f, -10.0f),
        feng::Vector3(-10.0f, -5.4f, -20.0f),
        feng::Vector3(-20.0f, -5.4f, 10.0f),
        feng::Vector3(0.0f, 0.0f, 2.0f),
        feng::Vector3(1.0f, 2.0f, 2.0f),
        feng::Vector3(-1.0f, -2.0f, 2.0f),
        feng::Vector3(0.5f, 0.5f, 2.5f)
    };

    feng::Vector3 planePos{0.f, -6.f, 0.f};

    std::vector<std::unique_ptr<feng::Entity>> dynamicObjects;
    std::vector<std::unique_ptr<feng::Entity>> staticObjects;
    std::unique_ptr<feng::Entity> planeEntity;
    std::map<feng::Entity *, bool> sceneOutline;
    std::vector<feng::Entity *> scene;
    feng::RenderProperties renderProperties;
    bool showDepth = false;

    std::unique_ptr<feng::Material> CreateGizmoMaterial()
    {
        if(showDepth)
        {
            return std::make_unique<feng::Material>(SRes::LoadShader(SRes::showDepthVsName, SRes::showDepthFsName));
        }

        return SRes::CreateFlatColorMaterial();
    }

    std::unique_ptr<feng::Entity> CreateCamera()
    {
        std::unique_ptr<feng::Entity> camEntity = std::make_unique<feng::Entity>("Camera");

        feng::Camera &camera = camEntity->AddComponent<feng::Camera>();
        camera.SetFovY(SCamController::Zoom);
        camera.SetAspectRatio(static_cast<float>(SApp::InitialWidth)/SApp::InitialHeight);
        camera.SetNearClipPlane(0.1f);
        camera.SetFarClipPlane(100.f);

        feng::Transform *camTransform = camEntity->GetComponent<feng::Transform>();
        camTransform->SetPosition(0.f, 3.5f, 3.f);
        camTransform->SetEuler(SCamController::camPitch, SCamController::camYaw, 0.f);

        SCamController::lastX = SApp::InitialWidth / 2.f;
        SCamController::lastY = SApp::InitialHeight / 2.f;
        feng::screen::ScreenWidth = SApp::InitialWidth;
        feng::screen::ScreenHeight = SApp::InitialHeight;

        return camEntity;
    }

    void AddLightGizmoRenderer(feng::Light &light)
    {
        feng::Entity *lightEntity = light.GetEntity();
        feng::Transform *cubeTransform = lightEntity->GetComponent<feng::Transform>();

        feng::Light::eType lightType = light.GetType();

        if(lightType == feng::Light::eType::Directional)
        {
            cubeTransform->SetScale(0.1f, 0.1f, 0.35f);
        }
        else if(lightType == feng::Light::eType::Point)
        {
            cubeTransform->SetScale(0.2f);
        }
        else
        {
            cubeTransform->SetScale(0.35f, 0.35f, 0.1f);
        }

        std::unique_ptr<feng::Material> flatColorMaterial = CreateGizmoMaterial();

        feng::MeshRenderer& meshRenderer = lightEntity->AddComponent<feng::MeshRenderer>();
        meshRenderer.SetMesh(SRes::cubeMesh.get());
        meshRenderer.SetMaterial(flatColorMaterial.get());

        flatColorMaterial->SetVector3(feng::ShaderParams::MainColor.data(), light.GetColor().GetXyz());
        lightMaterials[&light] = std::move(flatColorMaterial);
    }

    std::unique_ptr<feng::Entity> CreateDirectLight()
    {
        std::unique_ptr<feng::Entity> lightEntity = std::make_unique<feng::Entity>("DirLight");

        feng::Light &light = lightEntity->AddComponent<feng::Light>();
        light.SetType(feng::Light::eType::Directional);
        light.SetColor(feng::Vector4{1.f, 0.95f, 0.8f, 1.f});
        light.SetIntensity(1.f);

        feng::Transform *lightTransform = lightEntity->GetComponent<feng::Transform>();
        lightTransform->SetPosition(0.f, 0.f, 4.f);
        lightTransform->SetRotation(feng::mat3::MakeRotationY(180), feng::eSpace::World);

        AddLightGizmoRenderer(light);

        return lightEntity;
    }

    std::unique_ptr<feng::Entity> CreatePointLight()
    {
        std::unique_ptr<feng::Entity> lightEntity = std::make_unique<feng::Entity>("PointLight");

        feng::Light &light = lightEntity->AddComponent<feng::Light>();
        light.SetType(feng::Light::eType::Point);
        light.SetRange(8.f);
        light.SetColor(feng::Vector4{1.f, 0.f, 0.f, 1.f});
        light.SetIntensity(1.5f);

        feng::Transform *lightTransform = lightEntity->GetComponent<feng::Transform>();

        constexpr bool isUp = true;
        if(isUp)
        {
            lightTransform->SetPosition(0.f, 4.f, -1.f);
        }
        else
        {
            lightTransform->SetPosition(0.f, -12.f, -1.f);
        }

        AddLightGizmoRenderer(light);

        return lightEntity;
    }

    std::unique_ptr<feng::Entity> CreateSpotLight()
    {
        std::unique_ptr<feng::Entity> lightEntity = std::make_unique<feng::Entity>("SpotLight");

        feng::Light &light = lightEntity->AddComponent<feng::Light>();
        light.SetType(feng::Light::eType::Spot);
        light.SetRange(12.f);
        light.SetColor(feng::Vector4{0.f, 0.f, 1.f, 1.f});
        light.SetIntensity(10.f);
        light.SetSpotAngle(feng::DegToRad(45));

        feng::Transform *lightTransform = lightEntity->GetComponent<feng::Transform>();
        constexpr bool isUp = true;

        if(isUp)
        {
            lightTransform->SetPosition(5.f, 4.f, -2.f);

            lightTransform->SetRotation(feng::mat3::MakeRotationY(90), feng::eSpace::World);
            lightTransform->SetRotation(feng::mat3::MakeRotationX(20), feng::eSpace::Self);
        }
        else
        {
            lightTransform->SetPosition(0.f, -8.f, 2.f);

            lightTransform->SetRotation(feng::mat3::MakeRotationX(-90), feng::eSpace::Self);
        }

        AddLightGizmoRenderer(light);

        return lightEntity;
    }

    std::unique_ptr<feng::Entity> CreateObject(
                                            const feng::Vector3& position,
                                            const std::string& name,
                                            feng::Mesh &mesh,
                                            feng::Material& material)
    {
        std::unique_ptr<feng::Entity> cube = std::make_unique<feng::Entity>(name);

        feng::Transform *cubeTransform = cube->GetComponent<feng::Transform>();
        cubeTransform->SetPosition(position);

        feng::MeshRenderer& meshRenderer = cube->AddComponent<feng::MeshRenderer>();
        meshRenderer.SetMesh(&mesh);

        feng::Material *finalMaterial = showDepth ? SRes::showDepthMaterial.get() : &material;
        meshRenderer.SetMaterial(finalMaterial);

        return cube;
    }

    void CreateObjects()
    {
        SObjects::camEntity = SObjects::CreateCamera();
        SObjects::directLightEntity = SObjects::CreateDirectLight();
        SObjects::pointLightEntity = SObjects::CreatePointLight();
        SObjects::spotLightEntity = SObjects::CreateSpotLight();

        // Cubes.
        for(int32_t i = 0; i < cubePositions.size(); ++i)
        {
            const feng::Vector3& position = cubePositions[i];
            std::string name = "cube " + std::to_string(i);
            feng::Material *material = ((i % 2) == 0)
                ? SRes::diffTex1SpecTex2Material.get()
                : SRes::specularTexMaterial.get();
            std::unique_ptr<feng::Entity> entity = CreateObject(position, name, *SRes::cubeMesh, *material);
            dynamicObjects.push_back(std::move(entity));
        }

        // Grass.
        for(int32_t i = 0; i < vegetationPositions.size(); ++i)
        {
            const feng::Vector3& position = vegetationPositions[i];
            std::string name = "grass " + std::to_string(i);
            std::unique_ptr<feng::Entity> entity = CreateObject(position, name, *SRes::quadMesh, *SRes::grassMaterial);
            staticObjects.push_back(std::move(entity));
        }

        // Grass.
        for(int32_t i = 0; i < windowPositions.size(); ++i)
        {
            const feng::Vector3& position = windowPositions[i];
            std::string name = "window " + std::to_string(i);
            std::unique_ptr<feng::Entity> entity = CreateObject(position, name, *SRes::quadMesh, *SRes::windowMaterial);
            staticObjects.push_back(std::move(entity));
        }

        planeEntity = CreateObject(planePos, "Plane", *SRes::cubeMesh, *SRes::diffuseTexMaterial);
        feng::Transform *planeTransform = planeEntity->GetComponent<feng::Transform>();
        planeTransform->SetScale(40.f, 0.2f, 40.f);
    }

    void AddObjectsToScene()
    {
        sceneOutline[planeEntity.get()] = false;
        scene.push_back(planeEntity.get());

        for(std::unique_ptr<feng::Entity>& object : dynamicObjects)
        {
            sceneOutline[object.get()] = false;
            scene.push_back(object.get());
        }

        for(std::unique_ptr<feng::Entity>& object : staticObjects)
        {
            sceneOutline[object.get()] = false;
            scene.push_back(object.get());
        }

        sceneOutline[directLightEntity.get()] = true;
        scene.push_back(directLightEntity.get());
        sceneOutline[pointLightEntity.get()] = true;
        scene.push_back(pointLightEntity.get());
        sceneOutline[spotLightEntity.get()] = true;
        scene.push_back(spotLightEntity.get());
    }

    void InitRenderProperties()
    {
        renderProperties.cam = camEntity->GetComponent<feng::Camera>();
        renderProperties.ambientColorAndIntencity = feng::Vector4{1.f, 1.f, 1.f, 0.2f};
        renderProperties.directLight = directLightEntity->GetComponent<feng::Light>();
        renderProperties.pointLight = pointLightEntity->GetComponent<feng::Light>();
        renderProperties.spotLight = spotLightEntity->GetComponent<feng::Light>();
    }

    void CreateScene()
    {
        CreateObjects();
        AddObjectsToScene();
        InitRenderProperties();
    }

    void UpdateCamera()
    {
        feng::Camera *camera = SObjects::camEntity->GetComponent<feng::Camera>();

        camera->SetFovY(SCamController::Zoom);
        camera->SetAspectRatio(static_cast<float>(feng::screen::ScreenWidth)/feng::screen::ScreenHeight);
        camera->SetNearClipPlane(0.1f);
        camera->SetFarClipPlane(100.f);

        std::vector<feng::Entity*> outlined;
        for(feng::Entity *entity : SObjects::scene)
        {
            if(feng::MeshRenderer *renderer = entity->GetComponent<feng::MeshRenderer>())
            {
                feng::Material *material = renderer->GetMaterial();
                material->SetFloat(feng::ShaderParams::NearClipPlane.data(), 0.1f);
                material->SetFloat(feng::ShaderParams::FarClipPlane.data(), 100.f);
            }
        }
    }

    void UpdateObjects()
    {
        for(const std::unique_ptr<feng::Entity>& entity : dynamicObjects)
        {
            feng::Transform *transform = entity->GetComponent<feng::Transform>();

            float angle = 20.f * SApp::time;
            transform->SetEuler(0.f, 0.f, angle);
        }
    }

    void Update()
    {
        UpdateCamera();
        UpdateObjects();
    }
}

namespace SWindow
{
    constexpr float effectChangeInputDelay = 0.3f;
    float lastEffectChangeTime = 0.f;
    int32_t effectsCount = 0;
    int32_t appliedEffectIndex = -1;

    void ApplyNextPostEffect()
    {
        if(SApp::time - lastEffectChangeTime > effectChangeInputDelay)
        {
            lastEffectChangeTime = SApp::time;
            if(appliedEffectIndex + 1 < effectsCount)
            {
                ++appliedEffectIndex;
            }
            else
            {
                appliedEffectIndex = -1;
            }
        }
    }

    bool TryInitGlfw()
    {
        int initResult = glfwInit();
        if(initResult == GL_TRUE)
        {
            // Set required OpenGL version to 3.3.
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            // Set OpenGL context to be created under core-profile (without backward compatability features).
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            std::cout << "[TryInitGlfw]: Initialize GLFW with OpenGL version " << glfwGetVersionString() << "\n";

            return true;
        }

        std::cout << "[TryInitGlfw]: Cannot initialize GLFW\n";
        return false;
    }

    void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        feng::screen::ScreenWidth = width;
        feng::screen::ScreenHeight = height;
        glViewport(0, 0, width, height);
    }

    void MouseCallback(GLFWwindow* window, double x, double y)
    {
        static bool firstMouse = true;
        if (firstMouse) // initially set to true
        {
            SCamController::lastX = x;
            SCamController::lastY = y;
            firstMouse = false;
        }

        float xoffset = x - SCamController::lastX;
        float yoffset = SCamController::lastY - y; // reversed since y-coordinates range from bottom to top
        SCamController::lastX = x;
        SCamController::lastY = y;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        SCamController::camYaw += xoffset;
        SCamController::camPitch += yoffset;

        if(SCamController::camPitch > 89.0f)
        {
            SCamController::camPitch =  89.0f;
        }

        if(SCamController::camPitch < -89.0f)
        {
            SCamController::camPitch = -89.0f;
        }

        feng::Transform *camTransform = SObjects::camEntity->GetComponent<feng::Transform>();
        camTransform->SetRotation(feng::mat3::MakeRotationY(SCamController::camYaw), feng::eSpace::World);
        camTransform->SetRotation(feng::mat3::MakeRotationX(SCamController::camPitch), feng::eSpace::Self);
    }

    void ScrollCallback(GLFWwindow* window, double x, double y)
    {
        SCamController::Zoom -= static_cast<float>(y);
        if (SCamController::Zoom < 1.0f)
        {
            SCamController::Zoom = 1.0f;
        }

        if (SCamController::Zoom > 45.0f)
        {
            SCamController::Zoom = 45.0f;
        }
    }

    void ProcessWindowInput(GLFWwindow &window)
    {
        float speedMultiplier = 1;
        if(glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            speedMultiplier = 2;
        }
        else if (glfwGetKey(&window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            speedMultiplier = 0.5f;
        }

        if(glfwGetKey(&window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(&window, true);
        }

        if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
        {
            feng::Transform *camTransform = SObjects::camEntity->GetComponent<feng::Transform>();
            feng::Vector3 forward = camTransform->GetForward();
            camTransform->Move(SCamController::cameraSpeed * SApp::deltaTime * speedMultiplier * forward);
        }

        if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
        {
            feng::Transform *camTransform = SObjects::camEntity->GetComponent<feng::Transform>();
            feng::Vector3 forward = camTransform->GetForward();
            camTransform->Move(-SCamController::cameraSpeed * SApp::deltaTime * speedMultiplier * forward);
        }

        if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
        {
            feng::Transform *camTransform = SObjects::camEntity->GetComponent<feng::Transform>();
            feng::Vector3 right = camTransform->GetRight();
            camTransform->Move(-SCamController::cameraSpeed * SApp::deltaTime * speedMultiplier * right);
        }

        if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
        {
            feng::Transform *camTransform = SObjects::camEntity->GetComponent<feng::Transform>();
            feng::Vector3 right = camTransform->GetRight();
            camTransform->Move(SCamController::cameraSpeed * SApp::deltaTime * speedMultiplier * right);
        }

        if (glfwGetKey(&window, GLFW_KEY_O) == GLFW_PRESS)
        {
            ApplyNextPostEffect();
        }
    }

    GLFWwindow* CreateWindow()
    {
        GLFWwindow* window = glfwCreateWindow(SApp::InitialWidth, SApp::InitialHeight, "Sweet OpenGL Window", nullptr, nullptr);
        if (window != nullptr)
        {
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
            glfwSetCursorPosCallback(window, MouseCallback);
            glfwSetScrollCallback(window, ScrollCallback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glViewport(0, 0, SApp::InitialWidth, SApp::InitialHeight);
        }
        else
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        return window;
    }
}

namespace SRender
{
    feng::RenderPostProcessing postProcessing;
    feng::FrameBuffersPool buffersPool;
    feng::FrameBuffer frameBuffer;

    std::unique_ptr<feng::PostEffectDefinition> grayscaleEffect;
    std::vector<feng::PostEffectDefinition*> effects;

    void CreatePostEffectDefinitions()
    {
        std::unique_ptr<feng::Shader> grayscaleShader = SRes::LoadShader(
                                                                    SRes::postEffectVsName,
                                                                    SRes::grayscalePostEffectFsName);
        std::unique_ptr<feng::Material> grayscaleEffectMaterial = std::make_unique<feng::Material>(std::move(grayscaleShader));
        grayscaleEffect = std::make_unique<feng::PostEffectDefinition>(std::move(grayscaleEffectMaterial));

        effects.push_back(grayscaleEffect.get());
        SWindow::effectsCount = effects.size();
    }

    void PreparePostEffects()
    {
        if(SWindow::appliedEffectIndex >= 0)
        {
            feng::PostEffectDefinition *postEffect = effects[SWindow::appliedEffectIndex];
            postProcessing.SetPostEffect(*postEffect);
        }
        else
        {
            postProcessing.RemoveEffects();
        }

        if(postProcessing.HasPostEffects())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.Frame);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void ApplyPostEffects()
    {
        if(postProcessing.HasPostEffects())
        {
            postProcessing.ApplyPostEffects(frameBuffer);
        }
    }

    void InitRender()
    {
        feng::Debug::LogRenderInfoOpenGL();
        feng::Debug::LogMessage("Initialize render.");

        SRes::LoadResources();
        Print_Errors_OpengGL();

        SObjects::CreateScene();
        CreatePostEffectDefinitions();
#ifdef __APPLE__
        frameBuffer = buffersPool.CreateBuffer(2 * feng::screen::ScreenWidth, 2 * feng::screen::ScreenHeight, true);
#else
        frameBuffer = buffersPool.CreateBuffer(feng::screen::ScreenWidth, feng::screen::ScreenHeight, true);
#endif
        Print_Errors_OpengGL();

        if(SObjects::showDepth)
        {
            glClearColor(0.f, 0.5f, 0.f, 1.0f);
        }
        else
        {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS); // Default.
        glDepthMask(GL_TRUE); // Default.

        glEnable(GL_STENCIL_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // Default.
        glFrontFace(GL_CW);
        Print_Errors_OpengGL();
    }

    void RenderWithOutline(const std::vector<feng::Entity*>& outlined)
    {
        // Put 1s (ones) into stencil buffer for all drawn fragments.
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        for(feng::Entity *entity : outlined)
        {
            if(feng::MeshRenderer *renderer = entity->GetComponent<feng::MeshRenderer>())
            {
                renderer->Draw(SObjects::renderProperties);
            }
        }

        // Make the stencil test fail for all 1s in stencil buffer (for all previously rendered fragments).
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);

        std::unique_ptr<feng::Material> outlineMaterial = SRes::CreateFlatColorMaterial();
        outlineMaterial->SetVector3(feng::ShaderParams::MainColor.data(), feng::Vector3::OneY);

        for(feng::Entity *entity : outlined)
        {
            if(feng::MeshRenderer *renderer = entity->GetComponent<feng::MeshRenderer>())
            {
                feng::Transform *transform = entity->GetComponent<feng::Transform>();
                feng::Material *material = renderer->GetMaterial();
                feng::Vector3 scale = transform->GetScale();

                renderer->SetMaterial(outlineMaterial.get());
                transform->SetScale(1.05 * scale);
                renderer->Draw(SObjects::renderProperties);

                transform->SetScale(scale);
                renderer->SetMaterial(material);
            }
        }

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    }

    void SortSceneByDistance()
    {
        feng::Transform *camTransform = SObjects::camEntity->GetComponent<feng::Transform>();
        const feng::Vector3 &camPosition = camTransform->GetPosition();
        auto compare = [&camPosition] (const feng::Entity* e1, const feng::Entity* e2)
        {
            const feng::Transform *transform1 = e1->GetComponent<feng::Transform>();
            const feng::Vector3 &position1 = transform1->GetPosition();
            float distanceSqr1 = feng::Vector3::DistanceSqr(camPosition, position1);

            const feng::Transform *transform2 = e2->GetComponent<feng::Transform>();
            const feng::Vector3 &position2 = transform2->GetPosition();
            float distanceSqr2 = feng::Vector3::DistanceSqr(camPosition, position2);

            return distanceSqr2 < distanceSqr1;
        };

        std::sort(SObjects::scene.begin(), SObjects::scene.end(), compare);
    }

    void Render()
    {
        PreparePostEffects();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        SortSceneByDistance();

        std::vector<feng::Entity*> outlined;

        for(feng::Entity* entity : SObjects::scene)
        {
            if(SObjects::sceneOutline[entity])
            {
                outlined.push_back(entity);
            }
            else if(feng::MeshRenderer *renderer = entity->GetComponent<feng::MeshRenderer>())
            {
                renderer->Draw(SObjects::renderProperties);
            }
        }

        RenderWithOutline(outlined);
        Print_Errors_OpengGL();

        ApplyPostEffects();
        Print_Errors_OpengGL();
    }
}

int main(int argc, const char * argv[])
{
    if(SWindow::TryInitGlfw())
    {
        GLFWwindow *window = SWindow::CreateWindow();

        SRender::InitRender();
        feng::Debug::LogMessage("Start loop.");

        while(!glfwWindowShouldClose(window))
        {
            SApp::UpdateTime();
            SWindow::ProcessWindowInput(*window);
            SObjects::Update();
            SRender::Render();
            glfwSwapBuffers(window);
            glfwPollEvents();

#ifdef __APPLE__
            static bool macMoved = false;

            if(!macMoved)
            {
                int x, y;
                glfwGetWindowPos(window, &x, &y);
                glfwSetWindowPos(window, ++x, y);
                macMoved = true;
            }
#endif
            Print_Errors_OpengGL();
        }

        glfwTerminate();
        return 0;
    }

    return 1;
}
