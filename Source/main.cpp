#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/MatrixUtils.h>
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

    const char *DiffuseTextureFsName = "DiffuseTextureFs.fs";
    const char *DiffuseTextureVsName = "DiffuseTextureVs.vs";
    const char *SpecularTextureFsName = "SpecularTextureFs.fs";
    const char *SpecularTextureVsName = "SpecularTextureVs.vs";

    const char *VertexDiffuseTextureFsName = "VertexDiffuseTextureFs.fs";
    const char *VertexDiffuseTextureVsName = "VertexDiffuseTextureVs.vs";
    const char *VertexSpecularTextureFsName = "VertexSpecularTextureFs.fs";
    const char *VertexSpecularTextureVsName = "VertexSpecularTextureVs.vs";

    const char *woodenContainerJpg = "wood_container.jpg";
    const char *brickWallJpg = "brick_wall.jpg";
    const char *awesomeFacePng = "awesomeface.png";

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
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f
    };

    const unsigned int rectIndices[]
    {
        0, 1, 3,
        1, 2, 3
    };

    std::unique_ptr<feng::TextureData> woodContainerTextureData;
    std::unique_ptr<feng::TextureData> brickWallTextureData;
    std::unique_ptr<feng::TextureData> awesomeFaceTextureData;

    std::unique_ptr<feng::Texture> woodContainerTexture;
    std::unique_ptr<feng::Texture> brickWallTexture;
    std::unique_ptr<feng::Texture> awesomeFaceTexture;

    std::unique_ptr<feng::Material> diffuseTexMaterial;
    std::unique_ptr<feng::Material> specularTexMaterial;

    std::unique_ptr<feng::Material> vertexDiffuseTexMaterial;
    std::unique_ptr<feng::Material> vertexSpecularTexMaterial;

    std::unique_ptr<feng::Material> flatColorMaterial;

    std::unique_ptr<feng::Mesh> cubeMesh;

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
        specularTexMaterial->SetFloat("uSpecularity", 0.8f);
        specularTexMaterial->SetFloat("uShininess", 32.0f);

        std::unique_ptr<feng::Shader> vertexDiffuseTextureShader = LoadShader(VertexDiffuseTextureVsName, VertexDiffuseTextureFsName);
        vertexDiffuseTexMaterial = std::make_unique<feng::Material>(std::move(vertexDiffuseTextureShader));
        vertexDiffuseTexMaterial->SetTexture(feng::ShaderParams::Texture0.data(), woodContainerTexture.get());

        std::unique_ptr<feng::Shader> vertexSpecularTextureShader = LoadShader(VertexSpecularTextureVsName, VertexSpecularTextureFsName);
        vertexSpecularTexMaterial = std::make_unique<feng::Material>(std::move(vertexSpecularTextureShader));
        vertexSpecularTexMaterial->SetTexture(feng::ShaderParams::Texture0.data(), brickWallTexture.get());
        vertexSpecularTexMaterial->SetFloat("uSpecularity", 0.8f);
        vertexSpecularTexMaterial->SetFloat("uShininess", 32.0f);

        std::unique_ptr<feng::Shader> flatColorShader = LoadShader(FlatColorVsName, FlatColorFsName);
        flatColorMaterial = std::make_unique<feng::Material>(std::move(flatColorShader));
    }

    void LoadMeshes()
    {
        uint32_t attributesValue = feng::eVertexAtributes::Position | feng::eVertexAtributes::Normal | feng::eVertexAtributes::Uv0;
        feng::eVertexAtributes attributes = static_cast<feng::eVertexAtributes>(attributesValue);
        cubeMesh = std::make_unique<feng::Mesh>(cube, attributes, feng::ePrimitiveType::Triangles);
    }

    void LoadResources()
    {
        LoadMaterials();
        LoadMeshes();
    }
}

namespace SApp
{
    constexpr uint32_t Width = 800;
    constexpr uint32_t Height = 600;

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
    float lastX = SApp::Width / 2.f;
    float lastY = SApp::Height / 2.f;
    float Zoom = 45;
    const float cameraSpeed = 3.f;
    float camPitch = 0.f;
    float camYaw = 0.f;
}

namespace SObjects
{
    std::unique_ptr<feng::Entity> camEntity;
    std::unique_ptr<feng::Entity> pointLightEntity;

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

    std::vector<std::unique_ptr<feng::Entity>> objects;

    std::vector<feng::Entity *> scene;

    feng::RenderProperties renderProperties;

    std::unique_ptr<feng::Entity> CreateCamera()
    {
        std::unique_ptr<feng::Entity> camEntity = std::make_unique<feng::Entity>("Camera");

        feng::Camera &camera = camEntity->AddComponent<feng::Camera>();
        camera.SetFovY(SCamController::Zoom);
        camera.SetAspectRatio(static_cast<float>(SApp::Width)/SApp::Height);
        camera.SetNearClipPlane(0.1f);
        camera.SetFarClipPlane(100.f);

        feng::Transform *camTransform = camEntity->GetComponent<feng::Transform>();
        camTransform->SetPosition(0.f, 0.f, 3.f);
        camTransform->SetEuler(SCamController::camPitch, SCamController::camYaw, 0.f);

        return camEntity;
    }

    void AddLightGizmoRenderer(feng::Light &light)
    {
        feng::Entity *lightEntity = light.GetEntity();
        feng::Transform *cubeTransform = lightEntity->GetComponent<feng::Transform>();
        cubeTransform->SetScale(0.2f);

        feng::MeshRenderer& meshRenderer = lightEntity->AddComponent<feng::MeshRenderer>();
        meshRenderer.SetMesh(SRes::cubeMesh.get());
        meshRenderer.SetMaterial(SRes::flatColorMaterial.get());

        SRes::flatColorMaterial->SetVector3(feng::ShaderParams::MainColor.data(), light.GetColor().GetXyz());
    }

    std::unique_ptr<feng::Entity> CreateLight()
    {
        std::unique_ptr<feng::Entity> lightEntity = std::make_unique<feng::Entity>("Light");

        feng::Light &light = lightEntity->AddComponent<feng::Light>();
        light.SetType(feng::Light::eType::Point);
        light.SetRange(8.f);
        light.SetColor(feng::Vector4{1.f, 0.9f, 0.5f, 1.f});
        light.SetIntensity(1.f);

        feng::Transform *lightTransform = lightEntity->GetComponent<feng::Transform>();
        lightTransform->SetPosition(0.f, 4.f, -1.f);

        AddLightGizmoRenderer(light);

        return lightEntity;
    }

    std::unique_ptr<feng::Entity> CreateCube(
                                            const feng::Vector3& position,
                                            const std::string& name,
                                            const feng::Mesh &mesh,
                                            const feng::Material& material)
    {
        std::unique_ptr<feng::Entity> cube = std::make_unique<feng::Entity>(name);

        feng::Transform *cubeTransform = cube->GetComponent<feng::Transform>();
        cubeTransform->SetPosition(position);

        feng::MeshRenderer& meshRenderer = cube->AddComponent<feng::MeshRenderer>();
        meshRenderer.SetMesh(&mesh);
        meshRenderer.SetMaterial(&material);

        return cube;
    }

    void CreateObjects()
    {
        SObjects::camEntity = SObjects::CreateCamera();
        SObjects::pointLightEntity = SObjects::CreateLight();

        for(int32_t i = 0; i < cubePositions.size(); ++i)
        {
            const feng::Vector3& position = cubePositions[i];
            std::string name = "cube " + std::to_string(i);
            const feng::Material *material = ((i % 2) == 0)
                ? SRes::diffuseTexMaterial.get()
                : SRes::specularTexMaterial.get();
            std::unique_ptr<feng::Entity> entity = CreateCube(position, name, *SRes::cubeMesh, *material);
            objects.push_back(std::move(entity));
        }
    }

    void AddObjectsToScene()
    {
        for(std::unique_ptr<feng::Entity>& object : objects)
        {
            scene.push_back(object.get());
        }

        scene.push_back(pointLightEntity.get());
    }

    void InitRenderProperties()
    {
        renderProperties.cam = camEntity->GetComponent<feng::Camera>();
        renderProperties.ambientColor = feng::Vector4{1.f, 1.f, 1.f, 0.5f};
        renderProperties.pointLight = pointLightEntity->GetComponent<feng::Light>();
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
        camera->SetAspectRatio(static_cast<float>(SApp::Width)/SApp::Height);
        camera->SetNearClipPlane(0.1f);
        camera->SetFarClipPlane(100.f);
    }

    void UpdateObjects()
    {
        for(const std::unique_ptr<feng::Entity>& entity : objects)
        {
            feng::Transform *transform = entity->GetComponent<feng::Transform>();

            float angle = 20.f * SApp::time;
            transform->SetEuler(0.f, 0.f, angle);
        }
    }

    void UpdateLight()
    {
        for(const std::unique_ptr<feng::Entity>& entity : objects)
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

namespace SMain
{
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
        float speedMultiplier = (glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 2 : 1;

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

        if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
        {
            feng::Transform *camTransform = SObjects::camEntity->GetComponent<feng::Transform>();
            feng::Vector3 right = camTransform->GetRight();
            camTransform->Move(SCamController::cameraSpeed * SApp::deltaTime * speedMultiplier * right);
        }
    }

    GLFWwindow* CreateWindow()
    {
        constexpr int width = 800;
        constexpr int height = 600;
        GLFWwindow* window = glfwCreateWindow(width, height, "Sweet OpenGL Window", nullptr, nullptr);
        if (window != nullptr)
        {
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
            glfwSetCursorPosCallback(window, MouseCallback);
            glfwSetScrollCallback(window, ScrollCallback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glViewport(0, 0, width, height);
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
    feng::Vector4 lightColor {1.f, 0.f, 0.f, 0.f};

    void InitRender()
    {
        feng::Debug::LogRenderInfoOpenGL();
        feng::Debug::LogMessage("Initialize render.");

        SRes::LoadResources();
        SObjects::CreateScene();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glEnable(GL_DEPTH_TEST);
    }

    void Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(feng::Entity *entity : SObjects::scene)
        {
            if(feng::MeshRenderer *renderer = entity->GetComponent<feng::MeshRenderer>())
            {
                renderer->Draw(SObjects::renderProperties);
            }
        }
    }
}

int main(int argc, const char * argv[])
{
    if(SMain::TryInitGlfw())
    {
        GLFWwindow *window = SMain::CreateWindow();

        SRender::InitRender();

        Print_Errors_OpengGL();
        feng::Debug::LogMessage("Start loop.");

        while(!glfwWindowShouldClose(window))
        {
            SApp::UpdateTime();
            SMain::ProcessWindowInput(*window);
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
