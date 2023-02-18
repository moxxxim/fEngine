#include <Feng/Core/Engine.hpp>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include <Feng/App/Globals.h>
#include <Feng/Math/Size.h>
#include <Feng/ScenesManager/RenderSystem.h>
#include <Feng/Utils/Debug.h>

#include <iostream>
#include <sstream>

namespace Feng
{
    namespace SEngine
    {
        constexpr const char* WindowName = "Sweet Window";
        static constexpr Size2ui InitiaScreenSize {1200u, 800u};
        static GLFWwindow* window = nullptr;
        static Vector2 mousePos;
        static Vector2 mouseScroll;
        
        static void MouseCallback(GLFWwindow* window, double x, double y)
        {
            mousePos.x = static_cast<float>(x);
            mousePos.y = static_cast<float>(y);
        }

        static void ScrollCallback(GLFWwindow* window, double x, double y)
        {
            mouseScroll.x = static_cast<float>(x);
            mouseScroll.y = static_cast<float>(y);
        }
        
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
        {
            Screen::ScreenSize = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
            
            std::stringstream sstream;
            sstream << WindowName << " ";
            sstream << width << " x " << height;
            
            glfwSetWindowTitle(window, sstream.str().c_str());
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
        
        static GLFWwindow* CreateWindow()
        {
            Screen::ScreenSize = InitiaScreenSize;
            glfwWindowHint(GLFW_SAMPLES, 4);
            GLFWwindow* window = glfwCreateWindow(Screen::ScreenSize.width, Screen::ScreenSize.height, WindowName, nullptr, nullptr);
            if (window)
            {
                glfwMakeContextCurrent(window);
                glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
                glfwSetCursorPosCallback(window, MouseCallback);
                glfwSetScrollCallback(window, ScrollCallback);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glViewport(0, 0, Screen::ScreenSize.width, Screen::ScreenSize.height);
            }
            else
            {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
            }

            return window;
        }
        
        static void RenderWithOutline(const std::vector<Entity*>& outlined)
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
        
        void FixWindow()
        {
#ifdef __APPLE__
            static bool macMoved = false;

            if(!macMoved)
            {
                int width, height;
                glfwGetWindowSize(SEngine::window, &width, &height);
                glfwSetWindowSize(SEngine::window, width + 16, height + 9);
                glViewport(0, 0, width + 16, height + 9);

                macMoved = true;
            }
#endif
        }
    }

    std::array<eKeyStatus, InputKey::eKey::Count> Engine::inputKeys {eKeyStatus::Pressed};
    Engine *Engine::instance = nullptr;
    float Engine::deltaTime = 0.f;
    float Engine::time = 0.f;
    bool Engine::shadowsEnabled = false;
    bool Engine::showDepth = false;
    bool Engine::cursorVisible = false;
    
    bool Engine::IsMultisampleEnabled()
    {
        return glIsEnabled(GL_MULTISAMPLE);
    }
    
    void Engine::SetMultisampleEnabled(bool enabled)
    {
        if(enabled)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
        }
    }
    
    Engine::Engine()
    {
        instance = this;
        
        if(SEngine::TryInitGlfw())
        {
            SEngine::window = SEngine::CreateWindow();
            SEngine::InitRender(showDepth);
        }
    }

    int32_t Engine::Run()
    {
        if(SEngine::window)
        {
            Debug::LogMessage("Start loop.");
            while(!glfwWindowShouldClose(SEngine::window))
            {
                SEngine::FixWindow();
                UpdateTime();
                UpdateInputKeys();
                Update();
                Render();
                ResetFrameTemporary();
                glfwSwapBuffers(SEngine::window);
                glfwPollEvents();

                Print_Errors_OpengGL();
                
                if(Engine::IsKeyPressed(InputKey::Kb_Escape))
                {
                    glfwSetWindowShouldClose(SEngine::window, true);
                }
            }

            glfwTerminate();
            return 0;
        }
        
        return 1;
    }
    
    void Engine::SetPostEffect(PostEffectDefinition *postEffect)
    {
        scene->GetRenderSystem()->SetPostEffect(postEffect);
    }
    
    void Engine::RemovePostEffect()
    {
        scene->GetRenderSystem()->RemovePostEffect();
    }
    
    Vector2 Engine::GetMousePos()
    {
        return SEngine::mousePos;
    }
    
    Vector2 Engine::GetMouseScroll()
    {
        return SEngine::mouseScroll;
    }
    
    void Engine::UpdateTime()
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - time;
        time = currentFrame;
    }

    void Engine::UpdateInputKeys()
    {
        inputKeys[InputKey::Kb_A] = (glfwGetKey(SEngine::window, GLFW_KEY_A) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_B] = (glfwGetKey(SEngine::window, GLFW_KEY_B) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_C] = (glfwGetKey(SEngine::window, GLFW_KEY_C) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_Left_Shift] = (glfwGetKey(SEngine::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_Left_Control] = (glfwGetKey(SEngine::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_Escape] = (glfwGetKey(SEngine::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_W] = (glfwGetKey(SEngine::window, GLFW_KEY_W) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_S] = (glfwGetKey(SEngine::window, GLFW_KEY_S) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_D] = (glfwGetKey(SEngine::window, GLFW_KEY_D) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_O] = (glfwGetKey(SEngine::window, GLFW_KEY_O) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_Up] = (glfwGetKey(SEngine::window, GLFW_KEY_UP) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_Down] = (glfwGetKey(SEngine::window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_Left] = (glfwGetKey(SEngine::window, GLFW_KEY_LEFT) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_Right] = (glfwGetKey(SEngine::window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_Q] = (glfwGetKey(SEngine::window, GLFW_KEY_Q) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_E] = (glfwGetKey(SEngine::window, GLFW_KEY_E) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
        inputKeys[InputKey::Kb_I] = (glfwGetKey(SEngine::window, GLFW_KEY_I) == GLFW_PRESS)
                    ? eKeyStatus::Pressed
                    : eKeyStatus::Released;
    }
    
    void Engine::ResetFrameTemporary()
    {
        SEngine::mouseScroll = Vector2::Zero;
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
    
    bool Engine::IsCursorVisible()
    {
        return cursorVisible;
    }
    
    void Engine::SetCursorVisible(bool visible)
    {
        cursorVisible = visible;

        int visibilityValue = cursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
        glfwSetInputMode(SEngine::window, GLFW_CURSOR, visibilityValue);
    }
}
