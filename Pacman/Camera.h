#pragma once
#include "S2D/S2D.h"

using namespace S2D;

class Camera {
private:
	float _cameraRotation = 45;

public:
	Camera();

	void CameraControl(Input::KeyboardState *keyState, int elapsedTime);

	float GetRotation();
	void SetRotation(float rot);
};