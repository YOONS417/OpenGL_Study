#include "Camera.h"

Camera::Camera(glm::vec3 startPos) {
	CamPosition = startPos;
	CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
	CamUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Firstmouse = true;
	yaw = -90.0f;
	pitch = 0.0f;
	FOV = 45.5f;
	lastX = Screen_Height / 2.0f;
	lastY = Screen_Width  / 2.0f;
	Sensitivity = 0.1f;
	speedUnit = 3.0f;
}

glm::mat4 Camera::ViewMatrix() const {
	return glm::lookAt(CamPosition, CamPosition + CamFront, CamUp);
}

void Camera::KeyboardControl(int pressedkey, float DeltaTime) {
	const float CamSpeed = speedUnit * DeltaTime;
	
	if (pressedkey == GLFW_KEY_UP)
		CamPosition +=  CamFront * CamSpeed;
	if (pressedkey == GLFW_KEY_DOWN)
		CamPosition -=  CamFront * CamSpeed;
	if (pressedkey == GLFW_KEY_RIGHT)
		CamPosition += glm::normalize(glm::cross(CamFront, CamUp)) * CamSpeed;
	if (pressedkey == GLFW_KEY_LEFT)
		CamPosition -= glm::normalize(glm::cross(CamFront, CamUp)) * CamSpeed;
	if (pressedkey == GLFW_KEY_SPACE)
		CamPosition += CamUp * CamSpeed;
	if (pressedkey == GLFW_KEY_LEFT_CONTROL)
		CamPosition -= CamUp * CamSpeed;
} 

void Camera::MouseControl(float xPos, float yPos) {
	if (Firstmouse) {
		lastX = xPos;
		lastY = yPos;
		Firstmouse = false;
	}
	float Xoffset = xPos - lastX;
	float Yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	Xoffset *= Sensitivity;
	Yoffset *= Sensitivity;
	yaw += Xoffset;
	pitch += Yoffset;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	CameraDirection();
}

void Camera::MouseScroll(float yPos) {
	FOV -= yPos;
	if (FOV > 60.0f) FOV = 60.0f;
	if (FOV < 1.0f) FOV = 1.0f;
}

void Camera::CameraDirection() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * sin(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	CamFront = glm::normalize(direction);
}