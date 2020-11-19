#include <iostream>

// Define to remove "deprecation" messages about OpenGL
#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
//#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#include <GLFW/glfw3.h>

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
}

namespace SRender
{
    const float triangle[]
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    float rectangle[] = {
        // first triangle
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left
        // second triangle
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    GLuint shader;
    GLuint shapeVao;

    GLuint CreateVertexShader()
    {
        // Shader code.

        static const char *diffuseShaderVs =
        R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
        )";

        // Create shader object.

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        if(vertexShader != 0)
        {
            // Set code for shader object and compile.

            glShaderSource(vertexShader, 1, &diffuseShaderVs, nullptr);
            glCompileShader(vertexShader);

            // Check shader compilation.
            GLint success;
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                constexpr int logSize = 512;
                char errorLog[logSize];
                glGetShaderInfoLog(vertexShader, logSize, nullptr, errorLog);
                std::cout << "[Render] Failed to compile vertex shader: " << errorLog;
                glDeleteShader(vertexShader);
            }
        }

        return vertexShader;
    }

    GLuint CreateFragmentShader()
    {
        const char *diffuseShaderFs =
        R"(
        #version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
        )";

        // Create shader object.

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        if(fragmentShader != 0)
        {
            // Set code for shader object and compile.

            glShaderSource(fragmentShader, 1, &diffuseShaderFs, nullptr);
            glCompileShader(fragmentShader);

            // Check shader compilation.
            GLint success;
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                constexpr int logSize = 512;
                char errorLog[logSize];
                glGetShaderInfoLog(fragmentShader, logSize, nullptr, errorLog);
                std::cout << "[Render] Failed to compile fragment shader: " << errorLog;
                glDeleteShader(fragmentShader);
            }
        }

        return fragmentShader;
    }

    GLuint CreateShader()
    {
        GLuint vertexShader = CreateVertexShader();
        if(vertexShader == 0)
        {
            return 0;
        }

        GLuint fragmentShader = CreateFragmentShader();
        if(fragmentShader == 0)
        {
            return 0;
        }

        GLuint shaderProgram = glCreateProgram();
        if(shaderProgram != 0)
        {
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            GLint success;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if(!success)
            {
                constexpr int logSize = 512;
                char errorLog[logSize];
                glGetProgramInfoLog(shaderProgram, logSize, nullptr, errorLog);
                std::cout << "[Render] Failed to create shader program: " << errorLog;
                glDeleteProgram(shaderProgram);
            }
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }

    GLuint CreateVao(const float* triangles, int verticesCount)
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
        glBufferData(GL_ARRAY_BUFFER, 3 * verticesCount * sizeof(float), triangles, GL_STATIC_DRAW);

        // Setup vertex alignment in vertex buffer.

        /*
         1 - position of vertex attribute in shader: in our case this is position with location 0.
         2 - number of components in vertex attribute: in our case this is 3 (Vector3 dimension).
         3 - data type of each component in array.
         4 - whether data should be normalized (clamed to range [-1, 1], or [0, 1]). WTF????
         5 - offset between consecutive vertex attributes. Can be 0 in our case (indicates that attributes tightly packed).
         6 - offset of the first argument in array.
         */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // Enable vertex attribute in location 0.
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        return vao;
    }

    void InitRender()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Setup shader: create, compile and link vertex and fragment shaders.
        shader = CreateShader();
        shapeVao = CreateVao(rectangle, 6);

        // uncomment this call to draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void Render(GLuint vao, int verticesCount)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, verticesCount);
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
            SRender::Render(SRender::shapeVao, 6);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

    return 1;
}
