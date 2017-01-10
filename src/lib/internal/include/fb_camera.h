#ifndef CAMERA_H
#define CAMERA_H

#include "fb_export_spec.h"
#include <glew.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#ifdef _WIN32
class FB_ENGINE_API Camera
#else
class Camera
#endif
{

public:
	Camera(float hFov, float widthRatio, float heightRatio);
	~Camera();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	void Move(glm::vec3 translation);
	void UpdateViewMatrix();


private:
	glm::mat4 _projection;
	glm::mat4 _view;
	glm::vec3 _position;
	glm::vec3 _target;
	glm::vec3 _up;

};
#endif

