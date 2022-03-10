#include <Feng/Core/Engine.hpp>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include <Feng/Utils/Debug.h>
#include <Feng/App/Globals.h>

#include <iostream>

namespace Feng
{
    namespace
    {
        static constexpr uint32_t InitialWidth = 800;
        static constexpr uint32_t InitialHeight = 600;
        static GLFWwindow* window = nullptr;
        static Vector2 mousePos;
        static Vector2 mouseScroll;
        
        void MouseCallback(GLFWwindow* window, double x, double y)
        {
            mousePos.x = static_cast<float>(x);
            mousePos.y = static_cast<float>(y);
        }

        void ScrollCallback(GLFWwindow* window, double x, double y)
        {
            mouseScroll.x = static_cast<float>(x);
            mouseScroll.y = static_cast<float>(y);
        }
        
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
        {
            Screen::ScreenWidth = width;
            Screen::ScreenHeight = height;
            glViewport(0, 0, width, height);
        }
        
        static bool TryInitGlfw()
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
        
        static void InitRender(bool showDepth)
        {
            Debug::LogRenderInfoOpenGL();
            Debug::LogMessage("Initialize render.");

            Print_Errors_OpengGL();

            if(showDepth)
            {
                glClearColor(0.f, 0.5f, 0.f, 1.0f);
            }
            else
            {
                glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            }

            // Depth testing.
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS); // Default.
            glDepthMask(GL_TRUE); // Default.

            // Stencil testing.
            glEnable(GL_STENCIL_TEST);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Default
            glEnable(GL_MULTISAMPLE);

            Print_Errors_OpengGL();
        }
        
        GLFWwindow* CreateWindow()
        {
            Screen::ScreenWidth = InitialWidth;
            Screen::ScreenHeight = InitialHeight;
            glfwWindowHint(GLFW_SAMPLES, 4);
            GLFWwindow* window = glfwCreateWindow(InitialWidth, InitialHeight, "Sweet Window", nullptr, nullptr);
            if (window)
            {
                glfwMakeContextCurrent(window);
                glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
                glfwSetCursorPosCallback(window, MouseCallback);
                glfwSetScrollCallback(window, ScrollCallback);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glViewport(0, 0, InitialWidth, InitialHeight);
            }
            else
            {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
            }

            return window;
        }
        
        void RenderWithOutline(const std::vector<Entity*>& outlined)
        {
            /*
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

            std::unique_ptr<feng::Material> outlineMaterial = test::CreateFlatColorMaterial();
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
            */
        }
    }

    std::array<eKeyStatus, InputKey::eKey::Count> Engine::inputKeys {eKeyStatus::Pressed};
    Engine *Engine::instance = nullptr;
    float Engine::deltaTime = 0.f;
    float Engine::time = 0.f;
    bool Engine::showDepth = false;
    
    Engine::Engine()
    {
        instance = this;
        
        if(TryInitGlfw())
        {
            window = CreateWindow();
            InitRender(showDepth);
        }
    }

    int32_t Engine::Run()
    {
        if(window)
        {
            Debug::LogMessage("Start loop.");
            while(!glfwWindowShouldClose(window))
            {
                UpdateTime();
                UpdateInputKeys();
                Update();
                Render();
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
                
                if(Engine::IsKeyPressed(InputKey::Kb_Escape))
                {
                    glfwSetWindowShouldClose(window, true);
                }
            }

            glfwTerminate();
            return 0;
        }
        
        return 1;
    }
    
    void Engine::SetPostEffect(PostEffectDefinition *postEffect)
    {
        scene->SetPostEffect(postEffect);
    }
    
    void Engine::RemovePostEffect()
    {
        scene->RemovePostEffect();
    }
    
    Vector2 Engine::GetMousePos()
    {
        return mousePos;
    }
    
    Vector2 Engine::GetMouseScroll()
    {
        return mouseScroll;
    }
    
    void Engine::UpdateTime()
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - time;
        time = currentFrame;
    }

    void Engine::UpdateInputKeys()
    {
        inputKeys[InputKey::Kb_Left_Shift] = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        
        inputKeys[InputKey::Kb_Left_Control] = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;

        inputKeys[InputKey::Kb_Escape] = (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;

        inputKeys[InputKey::Kb_W] = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;

        inputKeys[InputKey::Kb_S] = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;

        inputKeys[InputKey::Kb_A] = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;

        inputKeys[InputKey::Kb_D] = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;

        inputKeys[InputKey::Kb_O] = (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        
        inputKeys[InputKey::Kb_Up] = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        
        inputKeys[InputKey::Kb_Down] = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        
        inputKeys[InputKey::Kb_Left] = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        
        inputKeys[InputKey::Kb_Right] = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        
        inputKeys[InputKey::Kb_Q] = (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        
        inputKeys[InputKey::Kb_E] = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
    }

    void Engine::Update()
    {
        if(scene)
        {
            scene->Update(deltaTime);
        }
    }
    
    void Engine::Render()
    {
        if(scene)
        {
            scene->Draw();
        }
    }
}
