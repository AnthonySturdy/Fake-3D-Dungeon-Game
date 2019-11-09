#include"Camera.h"

Camera::Camera() {

}

void Camera::CameraControl(Input::KeyboardState *keyState, int elapsedTime) {
	if (keyState->IsKeyDown(Input::Keys::LEFT)) {
		SetRotation(GetRotation() - 0.1f * elapsedTime);
	} else if (keyState->IsKeyDown(Input::Keys::RIGHT)) {
		SetRotation(GetRotation() + 0.1f * elapsedTime);
	}
}

float Camera::GetRotation() {
	return _cameraRotation;
}

void Camera::SetRotation(float rot) {
	_cameraRotation = rot;
}