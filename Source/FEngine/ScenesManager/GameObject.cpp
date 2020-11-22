//#include <FEngine/ScenesManager/GameObject.h>
//
//#include <FEngine/ScenesManager/Scene.h>
//#include <FEngine/ScenesManager/GameObjectScript.h>
//#include <FEngine/ScenesManager/Camera.h>
//#include <FEngine/ScenesManager/Light.h>
//#include <FEngine/ScenesManager/Renderer.h>
//#include <FEngine/ScenesManager/RenderSystem.h>
//
//namespace fengine
//{
//    GameObject::GameObject(Scene *scene, RenderSystem *renderSystem) :
//        Object(ObjectType::SceneObject),
//        m_scene(scene),
//        m_renderSystem(renderSystem)
//    {
//    }
//
//    GameObject::~GameObject()
//    {
//        UnregisterCameras();
//        UnregisterRenderers();
//        UnregisterLights();
//        DeleteComponents();
//    }
//
//    Renderer * GameObject::GetRenderer() const
//    {
//        return GetComponent<Renderer>();
//    }
//
//    Camera * GameObject::GetCamera() const
//    {
//        return GetComponent<Camera>();
//    }
//
//    void GameObject::AddComponent(GameObjectComponent *component)
//    {
//        if (m_components.Contains(component))
//        {
//            return;
//        }
//
//        component->SetOwner(this);
//        m_components.Add(component);
//        TryRegisterToSystem(component);
//    }
//
//    void GameObject::RemoveComponent(GameObjectComponent *component)
//    {
//        if (!m_components.Contains(component))
//        {
//            return;
//        }
//
//        m_components.Remove(component);
//        TryUnregisterFromSystem(component);
//
//        delete component;
//    }
//
//    void GameObject::Destroy()
//    {
//        ScenesManager *scenesManager = ScenesManager::GetInstance();
//        Scene *scene = scenesManager->GetCurrentScene();
//
//        return scene->DestroyGameObject(this);
//    }
//
//    GameObject * GameObject::Create()
//    {
//        ScenesManager *scenesManager = ScenesManager::GetInstance();
//        Scene *scene = scenesManager->GetCurrentScene();
//
//        return scene->CreateGameObject();
//    }
//
//    void GameObject::Update(float deltaTime)
//    {
//        if (m_scripts.GetSize() > 0)
//        {
//            auto scriptsIterator = m_scripts.GetIterator();
//            do
//            {
//                GameObjectScript * script = scriptsIterator.GetCurrent();
//                script->Update(deltaTime);
//
//            } while (scriptsIterator.MoveNext());
//        }
//    }
//
//    void GameObject::TryRegisterToSystem(GameObjectComponent *component)
//    {
//        GameObjectScript *script = dynamic_cast<GameObjectScript*>(component);
//        if (script)
//        {
//            m_scripts.Add(script);
//            script->Start();
//        }
//        else
//        {
//            Renderer *renderer = dynamic_cast<Renderer*>(component);
//            if (renderer)
//            {
//                m_renderSystem->AddRenderer(renderer);
//            }
//            else
//            {
//                Camera *camera = dynamic_cast<Camera*>(component);
//                if (camera)
//                {
//                    m_renderSystem->SetCamera(camera);
//                }
//                else
//                {
//                    Light *light = dynamic_cast<Light*>(component);
//                    if (light)
//                    {
//                        m_renderSystem->AddLight(light);
//                    }
//                }
//            }
//        }
//    }
//
//    void GameObject::TryUnregisterFromSystem(GameObjectComponent *component)
//    {
//        GameObjectScript *script = dynamic_cast<GameObjectScript*>(component);
//        if (script)
//        {
//            m_scripts.Remove(script);
//        }
//        else
//        {
//            Renderer *renderer = dynamic_cast<Renderer*>(component);
//            if (renderer)
//            {
//                m_renderSystem->RemoveRenderer(renderer);
//            }
//            else
//            {
//                Camera *camera = dynamic_cast<Camera*>(component);
//                if (camera)
//                {
//                    m_renderSystem->RemoveCamera(camera);
//                }
//            }
//        }
//    }
//
//    void GameObject::UnregisterCameras()
//    {
//        LinkedList<Camera> cameras = GetComponents<Camera>();
//        auto camerasIterator = cameras.GetIterator();
//        do
//        {
//            Camera *camera = camerasIterator.GetCurrent();
//            m_renderSystem->RemoveCamera(camera);
//
//        } while (camerasIterator.MoveNext());
//    }
//
//    void GameObject::UnregisterRenderers()
//    {
//        LinkedList<Renderer> renderers = GetComponents<Renderer>();
//        auto renderersIterator = renderers.GetIterator();
//        do
//        {
//            Renderer *renderer = renderersIterator.GetCurrent();
//            m_renderSystem->RemoveRenderer(renderer);
//
//        } while (renderersIterator.MoveNext());
//    }
//
//    void GameObject::UnregisterLights()
//    {
//        LinkedList<Light> renderers = GetComponents<Light>();
//        auto iterator = renderers.GetIterator();
//        do
//        {
//            Light *light = iterator.GetCurrent();
//            m_renderSystem->RemoveLight(light);
//
//        } while (iterator.MoveNext());
//    }
//
//    void GameObject::DeleteComponents()
//    {
//        auto componentsIterator = m_components.GetIterator();
//        do
//        {
//            GameObjectComponent *component = componentsIterator.GetCurrent();
//            if (component)
//            {
//                delete component;
//            }
//
//        } while (componentsIterator.MoveNext());
//
//        m_components.Clear();
//        m_scripts.Clear();
//    }
//}
