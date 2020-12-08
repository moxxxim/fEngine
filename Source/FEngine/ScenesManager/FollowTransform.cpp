//#include <FEngine/ScenesManager/FollowTransform.h>
//
//#include <FEngine/ScenesManager/GameObject.h>
//#include <FEngine/ScenesManager/Scene.h>
//#include <FEngine/ScenesManager/ScenesManager.h>
//
//namespace feng
//{
//    void FollowTransform::Start()
//    {
//
//    }
//
//    void FollowTransform::Update(float deltaTime)
//    {
//        if (m_target)
//        {
//            Transform *targetTransform = m_target->GetTransform();
//            Transform *transform = GetTransform();
//            if (m_isFollowPosition)
//            {
//                Vector3 currentPosition = transform->GetPosition();
//                Vector3 targetPosition = targetTransform->GetPosition();
//                Vector3 position = currentPosition * (1 - m_smoothness) + targetPosition * m_smoothness;
//
//                transform->SetPosition(position);
//            }
//
//            if (m_isFollowRotation)
//            {
//
//            }
//        }
//    }
//
//    void FollowTransform::Serialize(SerializationNode& node) const
//    {
//        obj_id targetId = m_target ? m_target->GetId() : 0;
//
//        node.SetName("FollowTransform");
//        node.SetLong("Target", targetId);
//        node.SetBool("FollowPosition", m_isFollowPosition);
//        node.SetBool("FollowRotation", m_isFollowRotation);
//        node.SetFloat("Smoothness", m_smoothness);
//    }
//
//    void FollowTransform::Deserialize(const SerializationNode& node)
//    {
//        long targetId;
//        float smoothness;
//        node.TryGetLong("Target", targetId);
//        node.TryGetBool("FollowPosition", m_isFollowPosition);
//        node.TryGetBool("FollowRotation", m_isFollowRotation);
//        node.TryGetFloat("Smoothness", smoothness);
//
//        ScenesManager *scenesManager = feng::ScenesManager::GetInstance();
//        Scene *scene = scenesManager->GetCurrentScene();
//        m_target = scene->GetGameObject((obj_id)targetId);
//        if (smoothness > 1)
//        {
//            m_smoothness = 1;
//        }
//        else if (smoothness < 0)
//        {
//            m_smoothness = 0;
//        }
//        else
//        {
//            m_smoothness = smoothness;
//        }
//    }
//}
