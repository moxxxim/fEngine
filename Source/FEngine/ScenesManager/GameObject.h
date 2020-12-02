//#pragma once
//
//#include <FEngine/App/Object.h>
//#include <FEngine/ScenesManager/Transform.h>
//
//namespace fengine
//{
//    class Scene;
//    class GameObjectComponent;
//    class Camera;
//    class Renderer;
//    class RenderSystem;
//    class GameObjectScript;
//    class SceneSerializer;
//
//    class GameObject final : public Object
//    {
//        friend Scene;
//        friend SceneSerializer;
//
//    public:
//        bool IsEnabled() const;
//        void SetEnabled(bool isEnabled);
//	      Transform * GetTransform();
//        Renderer * GetRenderer() const;
//        Camera * GetCamera() const;
//
//        template <class TObject> TObject * GetComponent() const;
//        template <class TObject> LinkedList<TObject> GetComponents() const;
//        template <class TObject> TObject * AddComponent();
//        template <class TObject> void RemoveComponent();
//        void RemoveComponent(GameObjectComponent *component);
//
//        void Destroy() override;
//
//        static GameObject * Create();
//
//    private:
//        GameObject(Scene *scene, RenderSystem *renderSystem);
//        ~GameObject();
//
//        void UnregisterCameras();
//        void UnregisterRenderers();
//        void UnregisterLights();
//        void DeleteComponents();
//        void Update(float deltaTime);
//        void AddComponent(GameObjectComponent *component);
//        void TryRegisterToSystem(GameObjectComponent *component);
//        void TryUnregisterFromSystem(GameObjectComponent *component);
//
//        bool m_isEnabled = true;
//        Transform m_transform;
//        Scene *m_scene = nullptr;
//        RenderSystem *m_renderSystem = nullptr;
//
//        LinkedList<GameObjectComponent> m_components;
//        LinkedList<GameObjectScript> m_scripts;
//    };
//
//    inline bool GameObject::IsEnabled() const
//    {
//        return m_isEnabled;
//    }
//
//    inline void GameObject::SetEnabled(bool isEnabled)
//    {
//        m_isEnabled = isEnabled;
//    }
//
//    inline Transform * GameObject::GetTransform()
//    {
//        return &m_transform;
//    }
//
//    template <class TObject>
//    TObject * GameObject::GetComponent() const
//    {
//        auto iterator = m_components.GetIterator();
//        while (iterator.HasCurrent())
//        {
//            GameObjectComponent *component = iterator.GetCurrent();
//            TObject *tComponent = dynamic_cast<TObject*>(component);
//            if (tComponent)
//            {
//                return tComponent;
//            }
//
//            iterator.MoveNext();
//        }
//
//        return nullptr;
//    }
//
//    template <class TObject>
//    LinkedList<TObject> GameObject::GetComponents() const
//    {
//        LinkedList<TObject> tComponents;
//        auto iterator = m_components.GetIterator();
//        do
//        {
//            GameObjectComponent *component = iterator.GetCurrent();
//            TObject *tComponent = dynamic_cast<TObject*>(component);
//            if (tComponent)
//            {
//                tComponents.Add(tComponent);
//            }
//        }
//        while (iterator.MoveNext());
//
//        return tComponents;
//    }
//
//    template <class TObject>
//    TObject * GameObject::AddComponent()
//    {
//        if (std::is_base_of<GameObjectComponent, TObject>::value)
//        {
//            TObject *tComponent = new TObject();
//            AddComponent(tComponent);
//
//            return tComponent;
//        }
//
//        return nullptr;
//    }
//
//    template <class T>
//    void GameObject::RemoveComponent()
//    {
//        TResource *component = GetComponent<TResource>();
//        if (component)
//        {
//            RemoveComponent(component);
//        }
//    }
//}
