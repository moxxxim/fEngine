#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <FEngine/ResourcesManager/Shader.h>
#include <FEngine/ScenesManager/Transform.h>
#include <FEngine/ScenesManager/Camera.h>
#include <FEngine/Utils/Debug.h>
#include <Classes/TextureLoader.h>
#include <FEngine/Math/Matrix4.h>
#include <FEngine/Math/MatrixUtils.h>
#include <FEngine/Math/Vector3.h>

#include <cmath>
#include <sstream>
#include <array>

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

namespace SMain
{
    std::string BaseResourcesDir = "../../../Resources/";
    std::string BaseTexturesDir = BaseResourcesDir + "Textures/";
    std::string BaseShadersDir = BaseResourcesDir + "Shaders/";
    std::string BaseTempShadersDir = BaseShadersDir + "Temp/";
    constexpr uint32_t Width = 800;
    constexpr uint32_t Height = 600;
    float lastX = Width / 2.f;
    float lastY = Height / 2.f;
    float Zoom = 45;

    float deltaTime = 0.0f;    // Time between current frame and last frame
    float lastFrame = 0.0f;
    float mixValue = 0.f;
    const float cameraSpeed = 3.f;
    float camPitch = 0.f;
    float camYaw = 0.f;

    fengine::Transform camTransform;
    fengine::Camera cam;
    fengine::Matrix4 camViewMatrix;

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
            lastX = x;
            lastY = y;
            firstMouse = false;
        }

        float xoffset = x - lastX;
        float yoffset = lastY - y; // reversed since y-coordinates range from bottom to top
        lastX = x;
        lastY = y;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camYaw += xoffset;
        camPitch += yoffset;

        if(camPitch > 89.0f)
        {
            camPitch =  89.0f;
        }
        if(camPitch < -89.0f)
        {
            camPitch = -89.0f;
        }

        camTransform.Rotate(camPitch, camYaw, 0);
    }

    void ScrollCallback(GLFWwindow* window, double x, double y)
    {
        Zoom -= static_cast<float>(y);
        if (Zoom < 1.0f)
        {
            Zoom = 1.0f;
        }
        if (Zoom > 45.0f)
        {
            Zoom = 45.0f;
        }
    }

    void UpdateMixValue(bool increase)
    {
        float delta = increase ? 0.005 : -0.005;
        mixValue += delta;

        std::stringstream ss;
        ss << "Mix value: " << mixValue;
        fengine::Debug::LogError(ss.str());
    }

    void ProcessWindowInput(GLFWwindow &window)
    {
        float speedMultiplier = (glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 2 : 1;

        if(glfwGetKey(&window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(&window, true);
        }
        else if(glfwGetKey(&window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            UpdateMixValue(true);
        }
        else if(glfwGetKey(&window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            UpdateMixValue(false);
        }
        else if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
        {
            fengine::Vector3 forward = camTransform.GetForward();
            camTransform.Move(cameraSpeed * deltaTime * speedMultiplier * forward);
        }
        if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
        {
            fengine::Vector3 forward = camTransform.GetForward();
            camTransform.Move(-cameraSpeed * deltaTime * speedMultiplier * forward);
        }
        if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
        {
            fengine::Vector3 right = camTransform.GetRight();
            camTransform.Move(-cameraSpeed * deltaTime * speedMultiplier * right);
        }
        if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
        {
            fengine::Vector3 right = camTransform.GetRight();
            camTransform.Move(cameraSpeed * deltaTime * speedMultiplier * right);
        }
    }

    std::unique_ptr<fengine::Shader> LoadTempShader(const std::string& vsFileName, const std::string& fsFileName)
    {
        std::string vsFilePath = BaseTempShadersDir + vsFileName;
        std::string fsFilePath = BaseTempShadersDir + fsFileName;
        return fengine::LoadShader(vsFilePath, fsFilePath);
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

    class TextureData final
    {
    public:
        TextureData() = default;

        TextureData(unsigned char *aData, int32_t aWidth, int32_t aHeight, int32_t aChanelsCount)
        : data {aData}
        , width {aWidth}
        , height {aHeight}
        , chanelsCount {aChanelsCount}
        {}

        ~TextureData()
        {
            if(data != nullptr)
            {
                stbi_image_free(data);
            }
        }

        void Load(const std::string& textureName, bool flip)
        {
            std::string texturePath = SMain::BaseTexturesDir + textureName;
            stbi_set_flip_vertically_on_load(flip);
            stbi_uc *textureData = stbi_load(texturePath.c_str(), &width, &height, &chanelsCount, 0);
            data = static_cast<unsigned char*>(textureData);
        }

        unsigned char *data = nullptr;
        int32_t width;
        int32_t height;
        int32_t chanelsCount;
    };
}

namespace SRender
{
    // Shaders.

    const char *twoTexturesFsName = "TriangleTexture2Fs.fs";
    const char *ModelViewProjVsName = "ModelViewProjVs.vs";
    // Textures.

    const char *woodenContainerJpg = "wood_container.jpg";
    const char *brickWallJpg = "brik_wall.jpg";
    const char *awesomeFacePng = "awesomeface.png";

    // Geometry.

    const float rectTexturedCoords[] =
    {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   // top right front
        0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right front
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left front
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left front
    };

    float cube[] =
    {
            // position       // color         // uv
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };

    std::array<fengine::Vector3, 10> cubePositions = {
        fengine::Vector3( 0.0f,  0.0f,  0.0f),
        fengine::Vector3( 2.0f,  5.0f, -15.0f),
        fengine::Vector3(-1.5f, -2.2f, -2.5f),
        fengine::Vector3(-3.8f, -2.0f, -12.3f),
        fengine::Vector3( 2.4f, -0.4f, -3.5f),
        fengine::Vector3(-1.7f,  3.0f, -7.5f),
        fengine::Vector3( 1.3f, -2.0f, -2.5f),
        fengine::Vector3( 1.5f,  2.0f, -2.5f),
        fengine::Vector3( 1.5f,  0.2f, -1.5f),
        fengine::Vector3(-1.3f,  1.0f, -1.5f)
    };

    const unsigned int rectIndices[]
    {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // Loaded data.

    GLuint shapeVao;
    GLuint shapeEbo;
    GLuint textureObj1;
    GLuint textureObj2;
    GLuint textureObj3;
    std::unique_ptr<fengine::Shader> modelViewProjShader;

    GLuint smallTriangleVao1;
    GLuint smallTriangleVao2;

    SMain::TextureData woodenContainerTexture;
    SMain::TextureData brickWallTexture;
    SMain::TextureData awesomeFaceTexture;

    GLuint CreateVao(const float* triangles, int verticesCount, int stride)
    {
        // Generate and bind vertex array object.
        GLuint vao;
        glGenVertexArrays(1, &vao);

        // Generate vertex buffer object.
        GLuint vbo;
        glGenBuffers(1, &vbo);

        // Vind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(vao);

        // Bind buffer object.

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 3 * stride, triangles, GL_STATIC_DRAW);

        // Setup vertex alignment in vertex buffer.

        /*
         1 - position of vertex attribute in shader: in our case this is position with location 0.
         2 - number of components in vertex attribute: in our case this is 3 (Vector3 dimension).
         3 - data type of each component in array.
         4 - whether data should be normalized (clamed to range [-1, 1], or [0, 1]). WTF????
         5 - offset between consecutive vertex attributes. Can be 0 in our case (indicates that attributes tightly packed).
         6 - offset of the first argument in array.
         */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

        // Enable vertex attribute in location 0.
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vao;
    }

    GLuint CreateVaoWithColor(const float* vertices, int verticesCount, int stride)
    {
        // Generate and bind vertex array object.
        GLuint vao;
        glGenVertexArrays(1, &vao);

        // Generate vertex buffer object.
        GLuint vbo;
        glGenBuffers(1, &vbo);

        // Vind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        // Enable vertex attribute in location 0.
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(stride/2));
        glEnableVertexAttribArray(1);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vao;
    }

    GLuint CreateVaoWithColorAndTexture(const float* vertices, int verticesCount, int stride)
    {
        // Generate and bind vertex array object.
        GLuint vao;
        glGenVertexArrays(1, &vao);

        // Generate vertex buffer object.
        GLuint vbo;
        glGenBuffers(1, &vbo);

        // Vind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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
        const uint32_t vec2Size = sizeof(float) * 2;

        // Position.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        // Enable vertex attribute in location 0.
        glEnableVertexAttribArray(0);

        // Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(vec3Size));
        glEnableVertexAttribArray(1);

        // Texture.
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(2 * vec3Size));
        glEnableVertexAttribArray(2);

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

    void LoadShaders()
    {
        modelViewProjShader = SMain::LoadTempShader(SRender::ModelViewProjVsName, SRender::twoTexturesFsName);
    }

    void LoadTextures()
    {
        woodenContainerTexture.Load(woodenContainerJpg, false);
        brickWallTexture.Load(brickWallJpg, false);
        awesomeFaceTexture.Load(awesomeFacePng, true);
    }

    void InitializeTransforms()
    {
        SMain::camTransform.SetPosition(0.f, 0.f, 3.f);
        SMain::camTransform.SetRotationEuler(SMain::camPitch, SMain::camYaw, 0.f);

        fengine::Matrix4 globalCamTransform = SMain::camTransform.GetGlobalMatrix();
        globalCamTransform.TryInvert(SMain::camViewMatrix);

        SMain::cam.SetFovY(SMain::Zoom);
        SMain::cam.SetAspectRatio(static_cast<float>(SMain::Width)/SMain::Height);
        SMain::cam.SetNearClipPlane(0.1f);
        SMain::cam.SetFarClipPlane(100.f);
    }

    GLuint InitTextureObj(const SMain::TextureData& textureData, int wrap_s, int wrap_t, bool withAlpha)
    {
        if(textureData.data != nullptr)
        {
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                         GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         textureData.width,
                         textureData.height,
                         0,
                         (withAlpha ? GL_RGBA : GL_RGB),
                         GL_UNSIGNED_BYTE,
                         textureData.data);
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
            fengine::Debug::LogError("No texture object created.");
            return 0;
        }
    }

    void InitRender()
    {
        fengine::Debug::LogMessage("Initialize render.");
        // Log maximal number of vertex attributes available

        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << "\n";

        // Specify clear value for color buffer: color to fill color buffer with after call to "glClear(GL_COLOR_BUFFER_BIT)".
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        InitializeTransforms();
        LoadShaders();
        LoadTextures();
        textureObj1 = InitTextureObj(woodenContainerTexture, GL_REPEAT, GL_REPEAT, false);
        textureObj2 = InitTextureObj(brickWallTexture, GL_REPEAT, GL_REPEAT, false);
        textureObj3 = InitTextureObj(awesomeFaceTexture, GL_REPEAT, GL_REPEAT, true);
        shapeVao = CreateVaoWithColorAndTexture(cube, 36, sizeof(cube) / 36);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        //glBindVertexArray(0);

        shapeEbo = CreateEbo(rectIndices, 6);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);
    }

    void UpdateCamera()
    {
        fengine::Matrix4 globalCamTransform = SMain::camTransform.GetGlobalMatrix();
        globalCamTransform.TryInvert(SMain::camViewMatrix);

        SMain::cam.SetFovY(SMain::Zoom);
        SMain::cam.SetAspectRatio(static_cast<float>(SMain::Width)/SMain::Height);
        SMain::cam.SetNearClipPlane(0.1f);
        SMain::cam.SetFarClipPlane(100.f);
    }

    fengine::Transform UpdateTransform(int32_t i)
    {
        fengine::Transform transform;
        transform.SetPosition(cubePositions[i]);
        float time = static_cast<float>(glfwGetTime());

        float angle = ((i % 3) == 0) ? 20.f * time : (20.f * i);
        transform.SetRotationEuler(angle, 0.f, 0.f);

        return transform;
    }

    void RenderEbo(GLuint ebo, GLuint vao, int indicesCount, fengine::Shader& shader)
    {
        shader.StartUse();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureObj1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureObj3);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        for(size_t i = 0; i < SRender::cubePositions.size(); ++i)
        {
            fengine::Transform modelTransform = UpdateTransform(i);
            fengine::Matrix4 modelTransformMatrix = modelTransform.GetGlobalMatrix();
            fengine::Matrix4 camProjectionMatrix = SMain::cam.GetProjectionMatrix();

            shader.SetUniformMatrix4("uModelMat", modelTransformMatrix);
            shader.SetUniformMatrix4("uViewMat", SMain::camViewMatrix);
            shader.SetUniformMatrix4("uProjMat", camProjectionMatrix);
            shader.SetUniformFloat("mixValue", SMain::mixValue);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}

int main(int argc, const char * argv[])
{
    if(SMain::TryInitGlfw())
    {
        GLFWwindow *window = SMain::CreateWindow();

        SRender::InitRender();
        glCheckError();

        fengine::Debug::LogMessage("Start loop.");

        SRender::modelViewProjShader->StartUse();
        SRender::modelViewProjShader->SetUniformInt("uTex1", 0);
        SRender::modelViewProjShader->SetUniformInt("uTex2", 1);
        SRender::modelViewProjShader->StopUse();

        SRender::modelViewProjShader->StartUse();
        SRender::modelViewProjShader->SetUniformInt("uTex1", 0);
        SRender::modelViewProjShader->SetUniformInt("uTex2", 1);
        SRender::modelViewProjShader->StopUse();

        while(!glfwWindowShouldClose(window))
        {
            float currentFrame = glfwGetTime();
            SMain::deltaTime = currentFrame - SMain::lastFrame;
            SMain::lastFrame = currentFrame;
            SMain::ProcessWindowInput(*window);

            SRender::UpdateCamera();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SRender::RenderEbo(SRender::shapeEbo, SRender::shapeVao, 6, *SRender::modelViewProjShader);
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
            glCheckError();
        }

        glfwTerminate();
        return 0;
    }

    return 1;
}
