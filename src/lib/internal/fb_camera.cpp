#include "fb_camera.h"

Camera::Camera(float hFov, float widthRatio, float heightRatio) {
	_projection = glm::perspective(glm::radians(hFov), widthRatio / heightRatio, 0.1f, 100.f);

	// Initialize the view matrix to standing
	_position = glm::vec3(0, 0, 8);
	_target = glm::vec3(0, 0, -12);
	_up = glm::vec3(0, 0, 0);
	_view = glm::lookAt(_position, _target, _up);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return _projection;
}

glm::mat4 Camera::GetViewMatrix() {
	return _view;
}

// This method recalculates the view matrix
// based on the current position, target and up values.
void Camera::UpdateViewMatrix() {
	glm::vec3 dir = _target - _position;
	_up = glm::cross(glm::vec3(1, 0, 0), dir);
	_view = glm::lookAt(_position, _target, _up);
}

void Camera::Move(glm::vec3 translation) {
	_position += translation;
	
	// For now we don't rotate and we interpret the 
	// translation as a strafing move
	_target.z += translation.z;
	_target.x += translation.x;
}