#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern const unsigned int Screen_Width;
extern const unsigned int Screen_Height;

class Camera {
public:
	glm::vec3 CamPosition;
	glm::vec3 CamFront;
	glm::vec3 CamUp;

	bool Firstmouse;
	float yaw;
	float pitch;
	float FOV;
	float lastX ;
	float lastY ;
	float Sensitivity;
	float speedUnit;

	Camera(glm::vec3 startPos);	//생성자

	glm::mat4 ViewMatrix() const;
	void KeyboardControl(int pressedkey, float DeltaTime);
	void MouseControl(float xPos, float yPos);
	void MouseScroll(float yoffset);

private:
	void CameraDirection();
};