//#pragma once
//
//#include "../../../Utilities/utilities.h"
//#include "Fog.h"
//
//namespace feng
//{
//
//class Texture;
//class Scene;
//class Mesh;
//
//class SceneSettings final
//{
//
//	friend Scene;
//
//public:
//
//	inline Fog& GetFog();
//	inline Vector4& GetAmbientLightColor();
//	inline void SetAmbientLightColor(const Vector4& color);
//
//private:
//
//	inline SceneSettings(Scene *m_scene);
//
//	Fog m_fog;
//	Vector4 m_ambientLightColor;
//	Scene *m_scene;
//};
//
//inline SceneSettings::SceneSettings(Scene *scene):
//	m_scene(scene)
//{
//}
//
//inline Fog& SceneSettings::GetFog()
//{
//	return m_fog;
//}
//
//inline Vector4& SceneSettings::GetAmbientLightColor()
//{
//	return m_ambientLightColor;
//}
//
//inline void SceneSettings::SetAmbientLightColor(const Vector4& color)
//{
//	m_ambientLightColor = color;
//}
//
//}
