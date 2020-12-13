#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <Feng/Math/Matrix4.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/Math/Vector3.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/Utils/Render/ShaderParams.h>
#include <Feng/ResourcesManager/TextureData.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Mesh.h>
#include <Feng/ResourcesManager/Texture.h>
#include <Feng/ScenesManager/Camera.h>
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

    const char *UnlitTexture2MixFsName = "Unlit/UnlitTexture2MixFs.fs";
    const char *UnlitTexture2MixVsName = "Unlit/UnlitTexture2MixVs.vs";
    const char *TextureFsName = "TextureFs.fs";
    const char *TextureVsName = "TextureVs.vs";
    const char *woodenContainerJpg = "wood_container.jpg";
    const char *awesomeFacePng = "awesomeface.png";

    std::vector<float> cube
    {
             // position        // uv
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    const unsigned int rectIndices[]
    {
        0, 1, 3,
        1, 2, 3
    };

    std::unique_ptr<feng::TextureData> woodContainerTextureData;
    std::unique_ptr<feng::TextureData> awesomeFaceTextureData;

    std::unique_ptr<feng::Material> unlitTex2MixMaterial;
    std::unique_ptr<feng::Material> texMaterial;

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

    void LoadMaterials()
    {
        std::unique_ptr<feng::Shader> unlitTexture2MixShader = LoadShader(UnlitTexture2MixVsName, UnlitTexture2MixFsName);
        std::unique_ptr<feng::Shader> textureShader = LoadShader(TextureVsName, TextureFsName);;

        unlitTex2MixMaterial = std::make_unique<feng::Material>(std::move(unlitTexture2MixShader));
        texMaterial = std::make_unique<feng::Material>(std::move(textureShader));

        woodContainerTextureData = LoadTexture(woodenContainerJpg, false);
        awesomeFaceTextureData = LoadTexture(awesomeFacePng, true);

        std::unique_ptr<feng::Texture> woodContainerTexture = std::make_unique<feng::Texture>(*woodContainerTextureData);
        std::unique_ptr<feng::Texture> awesomeFaceTexture = std::make_unique<feng::Texture>(*awesomeFaceTextureData);
    }

    void LoadMeshes()
    {
        uint32_t attributesValue = feng::eVertexAtributes::Position | feng::eVertexAtributes::Uv0;
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

    std::array<feng::Vector3, 10> cubePositions = {
        feng::Vector3( 0.0f,  0.0f,  0.0f),
        feng::Vector3( 2.0f,  5.0f, -15.0f),
        feng::Vector3(-1.5f, -2.2f, -2.5f),
        feng::Vector3(-3.8f, -2.0f, -12.3f),
        feng::Vector3( 2.4f, -0.4f, -3.5f),
        feng::Vector3(-1.7f,  3.0f, -7.5f),
        feng::Vector3( 1.3f, -2.0f, -2.5f),
        feng::Vector3( 1.5f,  2.0f, -2.5f),
        feng::Vector3( 1.5f,  0.2f, -1.5f),
        feng::Vector3(-1.3f,  1.0f, -1.5f)
    };

    std::vector<std::unique_ptr<feng::Entity>> objects;

    feng::RenderProperties renderProperties;

    std::unique_ptr<feng::Entity> CreateCamera()
    {
        std::unique_ptr<feng::Entity> camEntity = std::make_unique<feng::Entity>("Camera");
        feng::Camera &camera = camEntity->AddComponent<feng::Camera>();
        feng::Transform *camTransform = camEntity->GetComponent<feng::Transform>();
        camTransform->SetPosition(0.f, 0.f, 3.f);
        camTransform->SetEuler(SCamController::camPitch, SCamController::camYaw, 0.f);

        camera.SetFovY(SCamController::Zoom);
        camera.SetAspectRatio(static_cast<float>(SApp::Width)/SApp::Height);
        camera.SetNearClipPlane(0.1f);
        camera.SetFarClipPlane(100.f);

        return camEntity;
    }

    std::unique_ptr<feng::Entity> CreateCube(const feng::Vector3& position, const std::string& name, const feng::Mesh &mesh, const feng::Material& material)
    {
        std::unique_ptr<feng::Entity> cube = std::make_unique<feng::Entity>(name);

        feng::Transform *cubeTransform = cube->GetComponent<feng::Transform>();
        cubeTransform->SetPosition(position);

        feng::MeshRenderer& meshRenderer = cube->AddComponent<feng::MeshRenderer>();
        meshRenderer.SetMesh(SRes::cubeMesh.get());
        meshRenderer.SetMaterial(SRes::texMaterial.get());

        return cube;
    }

    void CreateObjects()
    {
        SObjects::camEntity = SObjects::CreateCamera();

        for(int32_t i = 0; i < cubePositions.size(); ++i)
        {
            const feng::Vector3& position = cubePositions[i];
            std::string name = "cube " + std::to_string(i);
            std::unique_ptr<feng::Entity> entity = CreateCube(position, name, *SRes::cubeMesh, *SRes::texMaterial);
            objects.push_back(std::move(entity));
        }
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
        else if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
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

    // Loaded data.

    GLuint shapeVao;
    GLuint shapeEbo;
    GLuint textureObj1;
    GLuint textureObj3;

    GLuint CreateVaoWithColorAndTexture(const float* vertices, int verticesCount, int stride)
    {
        // Generate and bind vertex array object.
        GLuint vao;
        glGenVertexArrays(1, &vao);

        // Generate vertex buffer object.
        GLuint vbo;
        glGenBuffers(1, &vbo);

        // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(vao);

        // Bind buffer object.

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, verticesCount * stride, vertices, GL_STATIC_DRAW);

        // Setup vertex alignment in vertex buffer.

        /*
         1 - position of vertex attribute in shader: in our case this is position with location 0.
         2 - number of components in vertex attribute: in our case this is 3 (Vector3 dimension).
         3 - data type of each component in array.
         4 - whether data should be normalized (clamed to range [-1, 1], or [0, 1]). WTF????
         5 - offset between consecutive vertex attributes. Can be 0 in our case (indicates that attributes tightly packed).
         6 - offset of the first argument in array.
         */

        const uint32_t vec3Size = sizeof(float) * 3;

        // Position.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        // Enable vertex attribute in location 0.
        glEnableVertexAttribArray(0);

        // Texture.
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(vec3Size));
        glEnableVertexAttribArray(1);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vao;
    }

    GLuint CreateEbo(const unsigned int* indices, unsigned int indicesCount)
    {
        GLuint ebo;
        glGenBuffers(1, &ebo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        return ebo;
    }

    GLuint InitTextureObj(const feng::TextureData& textureData, int wrap_s, int wrap_t, bool withAlpha)
    {
        if(textureData.IsValid())
        {
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                         GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         textureData.GetWidth(),
                         textureData.GetHeight(),
                         0,
                         (withAlpha ? GL_RGBA : GL_RGB),
                         GL_UNSIGNED_BYTE,
                         textureData.GetData());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            return texture;
        }
        else
        {
            feng::Debug::LogError("No texture object created.");
            return 0;
        }
    }

    void InitRender()
    {
        feng::Debug::LogRenderInfoOpenGL();
        feng::Debug::LogMessage("Initialize render.");

        SRes::LoadResources();
        SObjects::CreateObjects();

        // Specify clear value for color buffer: color to fill color buffer with after call to "glClear(GL_COLOR_BUFFER_BIT)".
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        textureObj1 = InitTextureObj(*SRes::woodContainerTextureData, GL_REPEAT, GL_REPEAT, false);
        textureObj3 = InitTextureObj(*SRes::awesomeFaceTextureData, GL_REPEAT, GL_REPEAT, true);
        shapeVao = CreateVaoWithColorAndTexture(SRes::cube.data(), 36, SRes::cube.size() * sizeof(float) / 36);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        //glBindVertexArray(0);

        shapeEbo = CreateEbo(SRes::rectIndices, 6);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);
    }

    void RenderEbo(GLuint ebo, GLuint vao, int indicesCount, const feng::Shader& shader)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.StartUse();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureObj1);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        for(std::unique_ptr<feng::Entity>& entity : SObjects::objects)
        {
            feng::Transform *transform = entity->GetComponent<feng::Transform>();
            feng::Matrix4 modelTransformMatrix = transform->GetGlobalMatrix();
            feng::Camera *camera = SObjects::camEntity->GetComponent<feng::Camera>();

            shader.SetUniformMatrix4(feng::ShaderParams::ModelMatrix.data(), modelTransformMatrix);
            shader.SetUniformMatrix4(feng::ShaderParams::ViewProjMatrix.data(), camera->GetViewProjectionMatrix());
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

        SRes::texMaterial->GetShader()->StartUse();
        SRes::texMaterial->GetShader()->SetUniformInt(feng::ShaderParams::Texture0.data(), 0);
        SRes::texMaterial->GetShader()->SetUniformVector3(feng::ShaderParams::DirLightColor.data(), SRender::lightColor.GetXyz());
        SRes::texMaterial->GetShader()->StopUse();

        while(!glfwWindowShouldClose(window))
        {
            SApp::UpdateTime();
            SMain::ProcessWindowInput(*window);
            SObjects::Update();
            SRender::Render();

            SRender::RenderEbo(SRender::shapeEbo, SRender::shapeVao, 6, *SRes::texMaterial->GetShader());
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
