#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <FEngine/ResourcesManager/Shader.h>
#include <FEngine/Utils/Debug.h>
#include <Classes/TextureLoader.h>

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
    uint32_t Width = 800;
    uint32_t Height = 600;


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

    GLFWwindow* CreateWindow()
    {
        constexpr int width = 800;
        constexpr int height = 600;
        GLFWwindow* window = glfwCreateWindow(width, height, "Sweet OpenGL Window", nullptr, nullptr);
        if (window != nullptr)
        {
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
            glViewport(0, 0, width, height);
        }
        else
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        return window;
    }

    float mixValue = 0.f;

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
    }

    std::unique_ptr<fengine::Shader> LoadTempShader(const std::string& vsFileName, const std::string& fsFileName)
    {
        std::string vsFilePath = BaseTempShadersDir + vsFileName;
        std::string fsFilePath = BaseTempShadersDir + fsFileName;
        return fengine::LoadShader(vsFilePath, fsFilePath);
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

    std::array<glm::vec3, 10> cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
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

    glm::mat4 modelTransformMatrix;
    glm::mat4 camProjectionMatrix;
    glm::mat4 camOrthoMatrix;
    glm::mat4 viewMatrix;

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
        camProjectionMatrix = glm::perspective(
                                        glm::radians(45.0f),
                                        static_cast<float>(SMain::Width)/SMain::Height,
                                        0.1f,
                                        100.0f);
        camOrthoMatrix = glm::ortho(
                                    0.0f,
                                    static_cast<float>(SMain::Width),
                                    0.0f,
                                    static_cast<float>(SMain::Height),
                                    0.1f,
                                    100.0f);

        modelTransformMatrix = glm::mat4(1.0f);
        modelTransformMatrix = glm::rotate(modelTransformMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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

    glm::mat4 CalculateTransform()
    {
        float time = static_cast<float>(glfwGetTime());
        float xPos = sin(time) * (2.f / 3);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(xPos, 0.0f, 0.0f));
        trans = glm::rotate(trans, time , glm::vec3(0.0f, 0.0f, 1.0f));

        return trans;
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

        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glm::vec3 camPos = glm::vec3(camZ, 0.f, camX);
        glm::vec3 camLookTarget = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);
        viewMatrix = glm::lookAt(camPos, camLookTarget, camUp);

        for(size_t i = 0; i < cubePositions.size(); ++i)
        {
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, cubePositions[i]);
            float angle = ((i % 3) == 0) ? glm::radians(static_cast<float>(20 * glfwGetTime())) : glm::radians(20.f * i);
            transform = glm::rotate(transform, angle, glm::vec3(0.5f, 1.0f, 0.0f));

            shader.SetUniformMatrix4("uModelMat", transform);
            shader.SetUniformMatrix4("uViewMat", SRender::viewMatrix);
            shader.SetUniformMatrix4("uProjMat", SRender::camProjectionMatrix);
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
            SMain::ProcessWindowInput(*window);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SRender::RenderEbo(SRender::shapeEbo, SRender::shapeVao, 6, *SRender::modelViewProjShader);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

    return 1;
}
