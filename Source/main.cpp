#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <FEngine/ResourcesManager/Shader.h>
#include <FEngine/Utils/Debug.h>
#include <Classes/TextureLoader.h>

#include <cmath>

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

    void ProcessWindowEscapeInput(GLFWwindow &window)
    {
        if(glfwGetKey(&window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(&window, true);
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

        void Load(const std::string& textureName)
        {
            std::string texturePath = SMain::BaseTexturesDir + textureName;
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

    const char *vertexColorVsName = "TriangleVertexColorVs.vs";
    const char *vertexColorFsName = "TriangleVertexColorFs.fs";
    const char *vertexColorUpsideDownVsName = "TriangleVertexColorUpsideDownVs.vs";
    const char *vertexColorWithOffsetVsName = "TriangleVertexColorWithOffsetVs.vs";
    const char *vertexColorFromPositionVsName = "TriangleVertexColorFromPositionVs.vs";
    const char *vertexColorFromPositionFsName = "TriangleVertexColorFromPositionFs.fs";

    const char *vertexColorTextureVsName = "TriangleVertexColorTextureVs.vs";
    const char *vertexColorTextureFsName = "TriangleVertexColorTextureFs.fs";


    // Textures.

    const char *woodenContainerJpg = "wood_container.jpg";
    const char *brickWallJpg = "brik_wall.jpg";

    // Geometry.

    const float triangle[]
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    const float triangleTextureCoordinates[]
    {
        0.0f, 0.0f,  // lower-left corner of texture
        1.0f, 0.0f,  // lower-right corner of texture
        0.5f, 1.0f  // top-center of texture
    };

    const float rectTexturedCoords[] =
    {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    const float triangleWithColors[]
    {
        0.5f, -0.5f, 0.0f,     1.0, 0.0, 0.0,  // Red vertex
        -0.5f, -0.5f, 0.0f,      0.0, 1.0, 0.0,  // Green vertex
        0.0f,  0.5f, 0.0f,      0.0, 0.0, 1.0,  // Blue vertex
    };

    const float rectangle[] = {
        // first triangle
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left
        // second triangle
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    const float twoTriangles[]
    {
        // Left triangle
        -0.5f, -0.5f, 0.0f,
        0.f, -0.5f, 0.0f,
        -0.25f, 0.f, 0.0f,

        // Right triangle
        0.f, 0.f, 0.0f,
        0.5f, 0.f, 0.0f,
        0.25f, 0.5f, 0.0f,
    };

    const unsigned int twoTrianglesIndices[]
    {
        0, 1, 2,
        3, 4, 5
    };

    const float smallTriangle1[]
    {
        -0.5f, -0.5f, 0.0f,
        0.f, -0.5f, 0.0f,
        -0.25f, 0.f, 0.0f,
    };

    const float smallTriangle2[]
    {
        0.f, 0.f, 0.0f,
        0.5f, 0.f, 0.0f,
        0.25f, 0.5f, 0.0f,
    };

    const float rectangleVertices[]
    {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    const unsigned int rectIndices[]
    {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // Loaded data.

    GLuint shapeVao;
    GLuint shapeEbo;
    GLuint textureObj;
    std::unique_ptr<fengine::Shader> vertexColorShader;
    std::unique_ptr<fengine::Shader> vertexColorUpsideDownShader;
    std::unique_ptr<fengine::Shader> vertexColorWithOffsetShader;
    std::unique_ptr<fengine::Shader> vertexColorFromPositionShader;
    std::unique_ptr<fengine::Shader> vertexColorTextureShader;

    GLuint smallTriangleVao1;
    GLuint smallTriangleVao2;

    SMain::TextureData woodenContainerTexture;
    SMain::TextureData brickWallTexture;

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
        vertexColorShader = SMain::LoadTempShader(SRender::vertexColorVsName, SRender::vertexColorFsName);
        vertexColorUpsideDownShader = SMain::LoadTempShader(
                                                            SRender::vertexColorUpsideDownVsName,
                                                            SRender::vertexColorFsName);
        vertexColorWithOffsetShader = SMain::LoadTempShader(
                                                            SRender::vertexColorWithOffsetVsName,
                                                            SRender::vertexColorFsName);
        vertexColorFromPositionShader = SMain::LoadTempShader(
                                                              SRender::vertexColorFromPositionVsName,
                                                              SRender::vertexColorFromPositionFsName);

        vertexColorTextureShader = SMain::LoadTempShader(
                                                         SRender::vertexColorTextureVsName,
                                                         SRender::vertexColorTextureFsName);
    }

    void LoadTextures()
    {
        woodenContainerTexture.Load(woodenContainerJpg);
        brickWallTexture.Load(brickWallJpg);
    }

    GLuint InitTextureObj(const SMain::TextureData& textureData)
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
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         textureData.data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

        LoadShaders();
        LoadTextures();
        textureObj = InitTextureObj(woodenContainerTexture);
        shapeVao = CreateVaoWithColorAndTexture(rectTexturedCoords, 4, sizeof(rectTexturedCoords) / 4);
        smallTriangleVao1 = CreateVao(smallTriangle1, 3, sizeof(smallTriangle1) / 3);
        smallTriangleVao2 = CreateVao(smallTriangle2, 3, sizeof(smallTriangle2) / 3);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        //glBindVertexArray(0);

        shapeEbo = CreateEbo(rectIndices, 6);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void RenderVao(GLuint vao, int verticesCount, fengine::Shader& shader)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.StartUse();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, verticesCount);
        shader.StopUse();
    }

    void RenderEbo(GLuint ebo, GLuint vao, int indicesCount, fengine::Shader& shader)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.StartUse();
        glBindTexture(GL_TEXTURE_2D, textureObj);
        glBindVertexArray(vao);

        // It's unneccessory if the EBO was bound while VAO was bound (and not unbound until VAO is unbound).
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
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
        while(!glfwWindowShouldClose(window))
        {
            SMain::ProcessWindowEscapeInput(*window);

//            float timeValue = glfwGetTime();
//            float offsetValue = std::sin(timeValue) / 2.0f;
//            SRender::vertexColorWithOffsetShader->SetUniformFloat("uOffset", offsetValue);
            //SRender::RenderVao(SRender::shapeVao, 4, *SRender::vertexColorTextureShader);
            SRender::RenderEbo(SRender::shapeEbo, SRender::shapeVao, 6, *SRender::vertexColorTextureShader);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

    return 1;
}
