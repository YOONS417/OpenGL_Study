#include <glad/glad.h>
#include <GLFW/glfw3.h> //include GLAD before GLFW
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


//초기화 루틴 : glfwInit() -> glfwCreateWindow() -> glfwMakeContextCurrent -> gladLoadGLLoader -> 렌더링 루프
int main() {
	glfwInit(); //반드시 가장 먼저 호출(GLFW_TRUE, GLFW_FALSE)
	//glfwWindowHint(int hint, int value);  창의 옵션
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpendlGL 3.3 버전을 사용
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// core profile을 사용 -> 오래된 기능들을 제거, 최신방식의 기능만 사용
	
	GLFWwindow* window = glfwCreateWindow(1000, 600, "OpenGL_Windoe", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //gladLoadGLLoader전에 사용
	
	//GLAD 라이브러리를 사용하여 OpenGL 함수 포인터를 초기화
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		// glfwGetProcAddress : OS와 GPU에 맞는 OpenGL함수의 실제 메모리 주소를 찾는 역학
		// GLADloadproc : glfwGetProcAddress가 반화하는 함수 타입을 GLAD에 맞는 타입으로 변화(Casting)
		// gladLoadGLLoader : 찾아낸 ㅏㅎㅁ수 주소들을 GLAD라이브러리에 전달(OpenGl명령어를 호출시 GLAD가 찾아둔 주소를 통해 GPU 명령으로 연결)
		std::cout << "Failed to initialze GLAD" << std::endl;
		return -1;
	}
	//OpenGL은 그래픽카드 드라이버에 의해 구현

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
	// framebuffer_size_callback를 GLFW시스템에 등록(register), 창 크기가 바뀔때만다 함수를 자동으로 실행


	while (!glfwWindowShouldClose(window)) //render loop , 창이 닫히지 않고 계속 반복
	{
		processInput(window);

		glClearColor(0.3f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);   //Double buffering기술을 사용하여 버퍼를 교체 : 깜빡거리는 현상(Flickergin)현상 방지
		//Front buffer : 현재 모니터에 출려괴고 있는 이미지
		//Back buffer : 프로그램이 다음 프레임에 보여주기 위해 그리고 있는 뒷면
		glfwPollEvents();	//발생한 event들을 확인 ( glfwSetFramebufferSizeCallback같은 합수들이 실행되는 시점)
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	//OpenGL 위도우의 크기가 변경될 때 호출하는 콜백 함수
	//윈도우의 크기를 조절하면 그리는 영역을 새 크기에 맞게 업데이트하기 위해
	//main안에서 GLFW에게 창 크기가 변하면 이 함수를 실행해줘 라고 알려야 함
}

void processInput(GLFWwindow* window) //키보드 입력 감지 후 프로그램 종료
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //GLFW_KEY_ESCAPE : ESC 
		glfwSetWindowShouldClose(window, true);//render loop의 while -> false
}
