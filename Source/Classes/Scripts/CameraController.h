#pragma once

#include <FEngine/ScenesManager/Transform.h>

class CameraController final
{
public:
	CameraController() = default;
	~CameraController() = default;

	void SetCamera(feng::Transform *cameraTransform);
    void TiltCam(float degree);
    void PanCam(float degree);
    void MoveForward(float distance);
    void MoveUp(float distance);

private:
	feng::Transform *cameraTransform = nullptr;
};

inline void CameraController::SetCamera(feng::Transform *aCameraTransform)
{
	cameraTransform = aCameraTransform;
}
