#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Transform.h"
enum CameraMode
{
	FREECAM,
	TARGET_VIEW
};

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;
	glm::vec3 target;
	float speed;
	float mouse_sensitivity;
	float dist_from_target;
	float pitch;
	float yaw;
	CameraMode mode;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	int windowWidth;
	int windowHeight;

	Camera(glm::vec3,glm::vec3,glm::vec3,glm::vec3,int,int);
	void toggleMode();
	void pan(float,float);
	void strafe(float,float,float,float);

	//virtual so you can override this behavior if you want
	//Base camera class will provide default implementation.
	virtual void update(float deltaTime);

private:
	void panModelCam(float, float);
	void panFreeCam(float, float);
	void strafeModelCam(float, float);
	void strafeFreeCam(float, float, float, float);
	void rotateCamera(float, float);
	void recalculateModelViewPosition();
	void recalculateFreeCamDirections();
	void updateViewAndProjectionMatricies();
};

