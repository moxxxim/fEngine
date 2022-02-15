#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <Classes/TempResouces.h>

#include <Feng/App/Globals.h>
#include <Feng/Math/MathUtils.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/Math/Vector3.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/Utils/Render/ShaderParams.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Light.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/MeshRenderer.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/ScenesManager/Scene.h>
#include <Feng/Utils/Debug.h>

#include <array>
#include <cmath>
#include <random>
#include <sstream>

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
    std::unique_ptr<feng::Scene> scene;
    std::map<feng::Light *, std::unique_ptr<feng::Material>> lightMaterials;
    feng::Entity *camEntity = nullptr;
    feng::MeshRenderer *instancedObject = nullptr;
    std::vector<feng::Matrix4> instances;

    std::array<feng::Vector3, 10> cubePositions =
    {
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

    std::vector<feng::Entity*> dynamicObjects;
    bool showDepth = false;

    feng::Entity* CreateCamera()
    {
        feng::Entity &camEntity = scene->CreateCamera();

        feng::Camera *camera = camEntity.GetComponent<feng::Camera>();
        camera->SetFovY(SCamController::Zoom);
        camera->SetAspectRatio(static_cast<float>(SApp::InitialWidth)/SApp::InitialHeight);
        camera->SetNearClipPlane(0.1f);
        camera->SetFarClipPlane(100.f);

        feng::Transform *camTransform = camEntity.GetComponent<feng::Transform>();
        camTransform->SetPosition(0.f, 3.5f, 3.f);
        camTransform->SetEuler(SCamController::camPitch, SCamController::camYaw, 0.f);

        return &camEntity;
    }

    std::unique_ptr<feng::Material> CreateLightMaterial(const feng::Vector4& color)
    {
        std::unique_ptr<feng::Material> material = CreateGizmoMaterial(showDepth);
        material->SetVector3(feng::ShaderParams::MainColor.data(), color.GetXyz());

        return material;
    }

    feng::Entity* CreateDirectLight()
    {
        feng::Vector4 color {1.f, 0.95f, 0.8f, 1.f};

        std::unique_ptr<feng::Material> material = CreateLightMaterial(color);
        feng::Entity& lightEntity = scene->CreateLight(
                                                    feng::Light::eType::Directional,
                                                    material.get(),
                                                    res.CubeMesh.get());

        feng::Light* light = lightEntity.GetComponent<feng::Light>();
        lightMaterials[light] = std::move(material);

        light->SetColor(color);
        light->SetIntensity(1.f);

        feng::Transform *lightTransform = lightEntity.GetComponent<feng::Transform>();
        lightTransform->SetPosition(0.f, 0.f, 4.f);
        lightTransform->SetRotation(feng::mat3::MakeRotationY(180), feng::eSpace::World);

        return &lightEntity;
    }

    feng::Entity* CreatePointLight()
    {
        feng::Vector4 color {1.f, 0.f, 0.f, 1.f};

        std::unique_ptr<feng::Material> material = CreateLightMaterial(color);
        feng::Entity& lightEntity = scene->CreateLight(
                                                        feng::Light::eType::Point,
                                                        material.get(),
                                                        res.CubeMesh.get());

        feng::Light* light = lightEntity.GetComponent<feng::Light>();
        lightMaterials[light] = std::move(material);

        light->SetRange(8.f);
        light->SetColor(color);
        light->SetIntensity(1.5f);

        feng::Transform *lightTransform = lightEntity.GetComponent<feng::Transform>();

        constexpr bool isUp = true;
        if(isUp)
        {
            lightTransform->SetPosition(0.f, 4.f, -1.f);
        }
        else
        {
            lightTransform->SetPosition(0.f, -12.f, -1.f);
        }

        return &lightEntity;
    }

    feng::Entity* CreateSpotLight()
    {
        feng::Vector4 color {0.f, 0.f, 1.f, 1.f};

        std::unique_ptr<feng::Material> material = CreateLightMaterial(color);
        feng::Entity& lightEntity = scene->CreateLight(
                                                         feng::Light::eType::Spot,
                                                         material.get(),
                                                         res.CubeMesh.get());

        feng::Light* light = lightEntity.GetComponent<feng::Light>();
        lightMaterials[light] = std::move(material);

        light->SetRange(12.f);
        light->SetColor(color);
        light->SetIntensity(10.f);
        light->SetSpotAngle(feng::DegToRad(45));

        feng::Transform *lightTransform = lightEntity.GetComponent<feng::Transform>();
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

        return &lightEntity;
    }

    feng::Entity* CreateObject(
                            const feng::Vector3& position,
                            const std::string& name,
                            feng::Mesh &mesh,
                            feng::Material& material)
    {
        feng::Material *finalMaterial = showDepth ? res.ShowDepthMaterial.get() : &material;
        feng::Entity& obj = scene->CreateMesh(finalMaterial, &mesh);

        feng::Transform *transform = obj.GetComponent<feng::Transform>();
        transform->SetPosition(position);

        return &obj;
    }

    feng::MeshRenderer* CreateInstancedObject(feng::Mesh &mesh, const std::vector<feng::Matrix4> &transforms)
    {
        feng::Entity& obj = scene->CreateMesh(res.DiffuseTexInstancedMaterial.get(), &mesh);

        feng::MeshRenderer *renderer = obj.GetComponent<feng::MeshRenderer>();
        renderer->SetInstanceTransforms(transforms);

        return renderer;
    }

    std::vector<feng::Matrix4> InitializeInstances()
    {
        constexpr uint32_t instancesCount = 10'000;

        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());
        std::uniform_real_distribution<float> angleDistribution(0.f, 359.9f);
        std::uniform_real_distribution<float> scaleDistribution(0.2f, 0.5f);
        std::uniform_real_distribution<float> radiusDistribution(20.f, 35.f);
        std::uniform_real_distribution<float> heightDistribution(1.5f, -1.5f);

        std::vector<feng::Matrix4> objects;
        objects.resize(instancesCount);

        for(uint32_t i = 0; i < instancesCount; ++i)
        {
            float scaleValue = scaleDistribution(generator);
            feng::Vector3 scale { scaleValue, scaleValue, scaleValue };

            float angle = angleDistribution(generator);
            float radius = radiusDistribution(generator);
            float height = heightDistribution(generator);
            feng::Vector3 translation = radius * (feng::Vector3::OneX * std::sin(angle) + feng::Vector3::OneZ * std::cos(angle))
                                        + height * feng::Vector3::OneY;

            objects[i] = feng::mat4::MakeTransformation(scale, feng::Matrix3::Identity, translation);
        }

        return objects;
    }

    void CreateObjects()
    {
        std::ignore = scene->CreateSkybox(res.SkyboxMaterial.get());
        SObjects::camEntity = SObjects::CreateCamera();
        std::ignore = SObjects::CreateDirectLight();
        std::ignore = SObjects::CreatePointLight();
        std::ignore = SObjects::CreateSpotLight();

        instances = InitializeInstances();
        instancedObject = CreateInstancedObject(*res.CubeMesh, instances);

        // Cubes.
        for(int32_t i = 0; i < cubePositions.size(); ++i)
        {
            const feng::Vector3& position = cubePositions[i];
            std::string name = "cube " + std::to_string(i);
            feng::Material *material = ((i % 2) == 0)
                ? res.DiffTex1SpecTex2Material.get()
                : res.SpecularTexMaterial.get();
            feng::Entity *entity = CreateObject(position, name, *res.CubeMesh, *material);
            dynamicObjects.push_back(entity);
        }

        // Grass.
        for(int32_t i = 0; i < vegetationPositions.size(); ++i)
        {
            const feng::Vector3& position = vegetationPositions[i];
            std::string name = "grass " + std::to_string(i);
            std::ignore = CreateObject(position, name, *res.QuadMesh, *res.GrassMaterial);
        }

        // Grass.
        for(int32_t i = 0; i < windowPositions.size(); ++i)
        {
            const feng::Vector3& position = windowPositions[i];
            std::string name = "window " + std::to_string(i);
            std::ignore = CreateObject(position, name, *res.QuadMesh, *res.WindowMaterial);
        }

        std::ignore = CreateObject(
                                feng::Vector3(0.f, 2.f, -2.5f),
                                "Reflective",
                                *res.CubeMesh,
                                *res.CubemapReflectiveMaterial);

        feng::Entity *planeEntity = CreateObject(planePos, "Plane", *res.CubeMesh, *res.DiffuseTexMaterial);
        feng::Transform *planeTransform = planeEntity->GetComponent<feng::Transform>();
        planeTransform->SetScale(40.f, 0.2f, 40.f);
    }

    void CreateScene()
    {
        SCamController::lastX = SApp::InitialWidth / 2.f;
        SCamController::lastY = SApp::InitialHeight / 2.f;
        feng::screen::ScreenWidth = SApp::InitialWidth;
        feng::screen::ScreenHeight = SApp::InitialHeight;

        scene = std::make_unique<feng::Scene>();
        scene->SetAmbientLight(feng::Vector4{1.f, 1.f, 1.f, 1.f}, 0.2f);
        CreateObjects();
    }

    void UpdateCamera()
    {
        feng::Camera *camera = SObjects::camEntity->GetComponent<feng::Camera>();

        camera->SetFovY(SCamController::Zoom);
        camera->SetAspectRatio(static_cast<float>(feng::screen::ScreenWidth)/feng::screen::ScreenHeight);
        camera->SetNearClipPlane(0.1f);
        camera->SetFarClipPlane(100.f);
    }

    void UpdateObjects()
    {
        for(feng::Entity *entity : dynamicObjects)
        {
            feng::Transform *transform = entity->GetComponent<feng::Transform>();

            float angle = 20.f * SApp::time;
            transform->SetEuler(0.f, 0.f, angle);
        }
    }

    void UpdateInstances()
    {
        constexpr float offsetFactor = 0.1f;
        for(size_t i = 0; i < instances.size(); i+=2)
        {
            instances[i].rows[3].z += offsetFactor * std::sin(SApp::time);
        }

        instancedObject->SetInstanceTransforms(instances);
    }

    void Update()
    {
        UpdateCamera();
        UpdateObjects();
        UpdateInstances();
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
        glfwWindowHint(GLFW_SAMPLES, 4);
        GLFWwindow* window = glfwCreateWindow(SApp::InitialWidth, SApp::InitialHeight, "Sweet OpenGL Window", nullptr, nullptr);
        if (window)
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
    void InitRender()
{
    feng::Debug::LogRenderInfoOpenGL();
    feng::Debug::LogMessage("Initialize render.");

    LoadResources();
    SWindow::effectsCount = res.Effects.size();
    Print_Errors_OpengGL();

    SObjects::CreateScene();
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Default

    glEnable(GL_MULTISAMPLE);

    Print_Errors_OpengGL();
}

    void RenderWithOutline(const std::vector<feng::Entity*>& outlined)
    {
        return;

        // Put 1s (ones) into stencil buffer for all drawn fragments.
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        for(feng::Entity *entity : outlined)
        {
            if(feng::MeshRenderer *renderer = entity->GetComponent<feng::MeshRenderer>())
            {
                //renderer->Draw(<render properties>);
            }
        }

        // Make the stencil test fail for all 1s in stencil buffer (for all previously rendered fragments).
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);

        std::unique_ptr<feng::Material> outlineMaterial = CreateFlatColorMaterial();
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
                //renderer->Draw(<render properties>);

                transform->SetScale(scale);
                renderer->SetMaterial(material);
            }
        }

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    }

    void Render()
    {
        if(SWindow::appliedEffectIndex >= 0)
        {
            feng::PostEffectDefinition *postEffect = res.Effects[SWindow::appliedEffectIndex];
            SObjects::scene->SetPostEffect(postEffect);
        }
        else
        {
            SObjects::scene->RemovePostEffect();
        }

        SObjects::scene->Draw();
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
