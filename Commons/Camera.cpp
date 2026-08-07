#include "Camera.h"

Camera::Camera(glm::vec3 startPos) {
	CamPosition = startPos;
	CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
	CamUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Firstmouse = true;
	yaw = -90.0f;
	pitch = 0.0f;
	FOV = 45.0f;
	lastX = Screen_Width / 2.0f;
	lastY = Screen_Height / 2.0f;
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
	if (Firstmouse) {	 //첫 마우스 입력 튐 방지
		lastX = xPos;	 //lastX,Y는 화면 중앙, 실제 커서는 다른 곳, 프레임 스왑 시 마우스 튐
		lastY = yPos;
		Firstmouse = false;
	}
	float Xoffset = xPos - lastX;
	float Yoffset = lastY - yPos; //스크린 좌표계는 왼쪽 위가 원점(0,0)
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

void Camera::CameraDirection() { 
	glm::vec3 direction; //위아래를 볼 때,바닥 방향으로 뻗어나가는 시선의 길이가 줄어드는 비율을 곱하기
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); //축소 필터 역할
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	CamFront = glm::normalize(direction);
} 

void Camera::MouseScroll(float yoffset) {  
	FOV -= yoffset;			// 휠을 굴릴 때 yoffset = +1.-1
	if (FOV > 60.0f) FOV = 60.0f;
	if (FOV < 1.0f) FOV = 1.0f;
}
 
void Camera::Rotate_Cam() {
	float radius = 15.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	CamPosition.x += camX;
	CamPosition.z += camZ;
	
	glm::vec3 target =  CamPosition + (CamFront * radius);
	CamFront = glm::normalize(target - CamPosition);


}
//front:0.0.-1	거리:10	  위치:0.0.7  ->  초점: 0.0.7 + 0.0.-10 = 0.0.-3     
/*   0.0.7 - 0.0.-3 = 0.0.10
초점 중심 카메라
1. 초점 설정
2. 원운동(각도 변수)
3. 초점에 시선 고정*/ 