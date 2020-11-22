#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <FEngine/ResourcesManager/Shader.h>

#include <cmath>

namespace SMain
{
    std::string BaseResourcesDir = "../../../Resources/";
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
}

namespace SRender
{
    const char *vertexColorVsName = "TriangleVertexColorVs.vs";
    const char *vertexColorFsName = "TriangleVertexColorFs.fs";
    const char *vertexColorUpsideDownVsName = "TriangleVertexColorUpsideDownVs.vs";
    const char *vertexColorWithOffsetVsName = "TriangleVertexColorWithOffsetVs.vs";
    const char *vertexColorFromPositionVsName = "TriangleVertexColorFromPositionVs.vs";
    const char *vertexColorFromPositionFsName = "TriangleVertexColorFromPositionFs.fs";

    const float triangle[]
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
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

    const unsigned int rectangleIndices[]
    {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    GLuint shapeVao;
    GLuint shapeEbo;
    std::unique_ptr<fengine::Shader> vertexColorShader;
    std::unique_ptr<fengine::Shader> vertexColorUpsideDownShader;
    std::unique_ptr<fengine::Shader> vertexColorWithOffsetShader;
    std::unique_ptr<fengine::Shader> vertexColorFromPositionShader;

    GLuint smallTriangleVao1;
    GLuint smallTriangleVao2;

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

    GLuint CreateVaoWithColor(const float* triangles, int verticesCount, int stride)
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        // Enable vertex attribute in location 0.
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(stride/2));
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
    }

    void InitRender()
    {
        // Log maximal number of vertex attributes available

        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << "\n";

        // Specify clear value for color buffer: color to fill color buffer with after call to "glClear(GL_COLOR_BUFFER_BIT)".
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        LoadShaders();
        shapeVao = CreateVaoWithColor(triangleWithColors, 3, sizeof(triangleWithColors) / 3);
        smallTriangleVao1 = CreateVao(smallTriangle1, 3, sizeof(smallTriangle1) / 3);
        smallTriangleVao2 = CreateVao(smallTriangle2, 3, sizeof(smallTriangle2) / 3);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        //glBindVertexArray(0);

        shapeEbo = CreateEbo(twoTrianglesIndices, 6);

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
        glBindVertexArray(vao);
        // It's unneccessory if the EBO was bound while VAO was bound (and not unbound until VAO is unbound).
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    }

    void RenderTwoSmallTriangles(fengine::Shader& firstShader, fengine::Shader& secondShader)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        firstShader.StartUse();
        glBindVertexArray(smallTriangleVao1);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(smallTriangle1) / 3);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        secondShader.StartUse();
        glBindVertexArray(smallTriangleVao2);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(smallTriangle2) / 3);
    }
}

int main(int argc, const char * argv[])
{
    if(SMain::TryInitGlfw())
    {
        GLFWwindow *window = SMain::CreateWindow();
        SRender::InitRender();

        // Render loop.
        while(!glfwWindowShouldClose(window))
        {
            SMain::ProcessWindowEscapeInput(*window);

//            float timeValue = glfwGetTime();
//            float offsetValue = std::sin(timeValue) / 2.0f;
//            SRender::vertexColorWithOffsetShader->SetUniformFloat("uOffset", offsetValue);
            SRender::RenderVao(SRender::shapeVao, 3, *SRender::vertexColorFromPositionShader);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

    return 1;
}
