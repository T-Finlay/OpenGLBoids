#include "Camera.h"
#include "Controller.h"
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 u, glm::vec3 t,int width,int height) {
	std::cout << "creating camera object" << std::endl;
	position = pos;
	forward = front;
	right = glm::normalize(glm::cross(forward,glm::vec3(0.f,1.f,0.f)));
	up = u;
	target = t;
	worldUp = glm::vec3(0.f,1.f,0.f);
	speed = .02f;
	mouse_sensitivity = 0.2f;
	dist_from_target = 10.f;
	pitch = 0.f;
	yaw = 90.f;
	mode = TARGET_VIEW;
	windowWidth = width;
	windowHeight = height;
	updateViewAndProjectionMatricies();
}

void Camera::toggleMode() {
	pitch *= -1;
	yaw = fmodf(yaw + 180.f, 360.f);
	switch (mode) {
	case TARGET_VIEW:
		mode = FREECAM;
		recalculateFreeCamDirections();
		break;
	case FREECAM:
		dist_from_target = glm::length(target - position);
		mode = TARGET_VIEW;
		recalculateModelViewPosition();
		break;
	}
}

void Camera::pan(float pitchDelta, float yawDelta) {
	switch (mode) {
	case TARGET_VIEW:
		panModelCam(pitchDelta, yawDelta);
		break;
	case FREECAM:
		panFreeCam(-pitchDelta, yawDelta);
		break;
	default:
		break;
	}
}

void Camera::strafe(float xDelta, float yDelta, float zDelta,float deltaTime) {
	switch (mode) {
	case TARGET_VIEW:
		strafeModelCam(zDelta,deltaTime);
		break;
	case FREECAM:
		strafeFreeCam(xDelta,yDelta,-zDelta,deltaTime);
		break;
	default:
		break;
	}
}

void Camera::update(float deltaTime) {
	Controller* controllerRef = Controller::getController();
	glm::vec3 strafes = glm::vec3(0.f, 0.f, 0.f);
	if (controllerRef->pollKeyDown(GLFW_KEY_W)) {
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
	if (controllerRef->pollKeyState(GLFW_KEY_N) == GLFW_PRESS) {
		toggleMode();
	}
	glm::vec2 mouseData = controllerRef->pollMouseDeltas();
	pan(mouseData.y,mouseData.x);
	strafe(strafes.x,strafes.y,strafes.z,deltaTime);
	updateViewAndProjectionMatricies();
}

void Camera::panModelCam(float pitchDelta, float yawDelta) {
	rotateCamera(pitchDelta, yawDelta);
	recalculateModelViewPosition();

	forward = glm::normalize(target - position);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::panFreeCam(float pitchDelta, float yawDelta) {
	rotateCamera(pitchDelta, yawDelta);
	recalculateFreeCamDirections();
}

void Camera::strafeModelCam(float zDelta,float deltaTime) {
	dist_from_target += zDelta * speed * deltaTime;
	if (dist_from_target < 1.f) {
		dist_from_target = 1.f;
	}
	recalculateModelViewPosition();
	forward = glm::normalize(target - position);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::strafeFreeCam(float xDelta, float yDelta, float zDelta, float deltaTime) {
	position += ((right * xDelta) + (worldUp * yDelta) + (forward * zDelta)) * speed * deltaTime;
}

void Camera::rotateCamera(float pitchDelta, float yawDelta){
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
}

void Camera::recalculateModelViewPosition() {
	float theta = glm::radians(yaw);
	float alpha = glm::radians(pitch);
	position = glm::vec3(cosf(theta) * cosf(alpha), sinf(alpha), sinf(theta) * cosf(alpha)) * dist_from_target;
}

void Camera::recalculateFreeCamDirections() {
	float theta = glm::radians(yaw);
	float alpha = glm::radians(pitch);

	forward = glm::vec3(cosf(theta) * cosf(alpha), sinf(alpha), sinf(theta) * cosf(alpha));
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::updateViewAndProjectionMatricies() {
	viewMatrix = glm::mat4(1.f);
	viewMatrix = glm::lookAt(position, position + forward,up);
	projectionMatrix = glm::mat4(1.f);
	projectionMatrix = glm::perspective(glm::radians(45.f), (float)windowWidth / (float)windowHeight, .01f, 500.f);
}
