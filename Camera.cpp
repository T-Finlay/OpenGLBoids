#include "Camera.h"
#include "Controller.h"
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 u, glm::vec3 t) {
	std::cout << "creating camera object" << std::endl;
	position = pos;
	forward = front;
	right = glm::normalize(glm::cross(forward,glm::vec3(0.f,1.f,0.f)));
	up = u;
	target = t;
	worldUp = glm::vec3(0.f,1.f,0.f);
	speed = .1f;
	mouse_sensitivity = 0.5f;
	dist_from_target = 10.f;
	pitch = 0.f;
	yaw = 90.f;
	mode = TARGET_VIEW;
}

void Camera::toggleMode() {
	//TODO:
}

void Camera::pan(float pitchDelta, float yawDelta) {
	//std::cout << "panning: y = " << pitchDelta << " x = " << yawDelta << std::endl;
	switch (mode) {
	case TARGET_VIEW:
		panModelCam(pitchDelta, yawDelta);
	case FREECAM:
		panFreeCam(pitchDelta, yawDelta);
	default:
		break;
	}
}

void Camera::strafe(float xDelta, float yDelta, float zDelta,float deltaTime) {
	switch (mode) {
	case TARGET_VIEW:
		strafeModelCam(zDelta,deltaTime);
	case FREECAM:
		strafeFreeCam(xDelta,yDelta,zDelta,deltaTime);
	default:
		break;
	}
}

void Camera::update(float deltaTime) {
	Controller* controllerRef = Controller::getController();

	glm::vec3 strafes = glm::vec3(0.f, 0.f, 0.f);

	if (controllerRef->pollKeyDown(GLFW_KEY_W)) {
		std::cout << "W down" << std::endl;
		strafes.z -= 1.f;
	}
	if (controllerRef->pollKeyDown(GLFW_KEY_A)) {
		strafes.x -= 1.f;
	}
	if (controllerRef->pollKeyDown(GLFW_KEY_S)) {

		strafes.z += 1.f;
	}
	if (controllerRef->pollKeyDown(GLFW_KEY_D)) {
		strafes.x += 1.f;
	}
	if (controllerRef->pollKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		strafes.y -= 1.f;
	}
	if (controllerRef->pollKeyDown(GLFW_KEY_SPACE)) {
		strafes.y += 1.f;
	}

	glm::vec2 mouseData = controllerRef->pollMouseDeltas();

	pan(mouseData.y,mouseData.x);
	strafe(strafes.x,strafes.y,strafes.z,deltaTime);
}

void Camera::panModelCam(float pitchDelta, float yawDelta) {
	yaw -= yawDelta * mouse_sensitivity;
	pitch -= pitchDelta * mouse_sensitivity;
	yaw = fmodf(yaw, 360.f);
	if (yaw < 0.0f) {
		yaw += 360.0f;
	}

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	
	recalculateModelViewPosition();

	forward = glm::normalize(target - position);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::panFreeCam(float, float)
{
}

void Camera::strafeModelCam(float zDelta,float deltaTime) {
	dist_from_target += zDelta * speed * deltaTime;
	recalculateModelViewPosition();
}

void Camera::strafeFreeCam(float, float, float, float)
{
}

void Camera::recalculateModelViewPosition() {
	float theta = glm::radians(yaw);
	float alpha = glm::radians(pitch);
	position = glm::vec3(cosf(theta) * cosf(alpha), sinf(alpha), sinf(theta) * cosf(alpha));
	position = position * dist_from_target;
}
