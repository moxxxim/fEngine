//#pragma once
//
//namespace feng
//{
//
//class Light;
//class RenderSystem;
//class Fog;
//class Camera;
//class ScenesManager;
//class Scene;
//class SceneSettings;
//
//class RenderProperties final
//{
//
//	friend RenderSystem;
//
//public:
//
//	const Fog& GetFog() const;
//	const Camera * GetCamera() const;
//
//	Vector4 GetAmbientLightColor() const;
//	inline int GetDirectLightsCount() const;
//	inline const Vector3 * GetDirectLightDirections() const;
//	inline const Vector4 * GetDirectLightColors() const;
//	inline const float * GetDirectLightIntensities() const;
//	inline int GetPointLightsCount() const;
//	inline const Vector3 * GetPointLightPositions() const;
//	inline const float * GetPointLightRanges() const;
//	inline const Vector4 * GetPointLightColors() const;
//	inline const float * GetPointLightIntensities() const;
//
//private:
//
//	RenderProperties(RenderSystem *renderSystem);
//	void Update();
//	void CacheDirectLight(Light *light);
//	void CachePointLight(Light *light);
//
//	RenderSystem *m_renderSystem = nullptr;
//
//	Vector<Vector3> m_directLightDirections;
//	Vector<Vector4> m_directLightColors;
//	Vector<float> m_directLightIntensities;
//	Vector<Vector3> m_pointLightPositions;
//	Vector<float> m_pointLightRanges;
//	Vector<Vector4> m_pointLightColors;
//	Vector<float> m_pointLightIntensities;
//};
//
//inline int RenderProperties::GetDirectLightsCount() const
//{
//	return m_directLightDirections.GetSize();
//}
//
//inline const Vector3 * RenderProperties::GetDirectLightDirections() const
//{
//	return &m_directLightDirections[0];
//}
//
//inline const Vector4 * RenderProperties::GetDirectLightColors() const
//{
//	return &m_directLightColors[0];
//}
//
//inline const float * RenderProperties::GetDirectLightIntensities() const
//{
//	return &m_directLightIntensities[0];
//}
//
//inline int RenderProperties::GetPointLightsCount() const
//{
//	return m_pointLightPositions.GetSize();
//}
//
//inline const Vector3 * RenderProperties::GetPointLightPositions() const
//{
//	return &m_pointLightPositions[0];
//}
//
//inline const float * RenderProperties::GetPointLightRanges() const
//{
//	return &m_pointLightRanges[0];
//}
//
//inline const Vector4 * RenderProperties::GetPointLightColors() const
//{
//	return &m_pointLightColors[0];
//}
//
//inline const float * RenderProperties::GetPointLightIntensities() const
//{
//	return &m_pointLightIntensities[0];
//}
//
//}
