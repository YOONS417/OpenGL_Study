#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderClass.h"
#include "stb_image.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_Callback(GLFWwindow* window, double xPos,double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int Screen_Width = 1200;
const unsigned int Screen_Height = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));   //카메라 생성, 위치:(0,0,10)

float DeltaTime = 0.0f; //카메라 이동 하드웨어 제한 방지(고정된 속도)
float LastFrame = 0.0f; 
bool isMouseOn, isKeypressed = false; // M키 설정

glm::vec3 SunPos(-5.0f, 5.0f, -5.0f); //Sun position
glm::vec3 SunLight(1.0f, 1.0f, 1.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(Screen_Width, Screen_Height, "Project_Lighting", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return  -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << " Failed to initialze GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);

    Shader SunLight_Shader("Shaders/sunlight.vert", "Shaders/sunlight.frag"); // 광원
    Shader LightingCube_Shader("Shaders/cube.vert", "Shaders/cube.frag");     //Cube Shader

    float cube_vert[] = {  // each point : 0 ~ 7
         // Fornt surface      //법선 
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  // left  bottom     = 0
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  // right  bottom    = 1
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  // right  top       = 2
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  // left  top        = 3
         // Right surface
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  // left  bottom     = 1
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  // right  bottom    = 5
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  // right  top       = 6
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  // left  top        = 2
         // Left surface
        -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  // left  bottom     = 4
        -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  // right  bottom    = 0
        -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  // right  top       = 3
        -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  // left  top        = 7
         // Top surface
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  // left  bottom     = 3
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  // right  bottom    = 2
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  // right  top       = 6
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  // left  top        = 7
         // Bottom surface
        -0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,  // left  bottom     = 0
         0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,  // right  bottom    = 1
         0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,  // right  top       = 5
        -0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,  // left  top        = 4
         // Back surface
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,  // left  bottom     = 4
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,  // right  bottom    = 5
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,  // right  top       = 6
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f   // left  top        = 7
    };
    unsigned int cube_indices[] = {  // indices : 정점 데이터 배열의 행 번호(0~23)
        0, 1,  2,  0, 2, 3,       // Fornt surface
        4, 5,  6,  4, 6, 7,       // Right surface
        8, 9, 10,  8,10,11,       // Left surface  
        12,13,14, 12,14,15,       //Top surface
        16,17,18, 16,18,19,       //Bottom surface    
        20,21,22, 20,22,23        //Back surface
    };

    unsigned int VBO, cubeVAO, EBO; 
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vert), cube_vert, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 각 면의 볍선벡터
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)) );
    glEnableVertexAttribArray(1);

    unsigned int sunVAO;
    glGenVertexArrays(1, &sunVAO);
    glBindVertexArray(sunVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
 
	// --Instruction--
	std::cout << "\n" << "================Camera Control================" << std::endl;
    std::string key[] = { "KEY_UP", "KEY_DOWN", "KEY_RIGHT", "KEY_LEFT", "SPACE_BAR", "CONTROL" ,"M", "Scroll"};
    std::string move[] = { "Forword", "Back", "Right", "Left", "Up" , "Down" ,"Mouse Camera On/Off", "Zoom in/out"};
    for (int i = 0; i < std::size(move); i++) {
        std::cout << key[i] << " : " << move[i] << std::endl;
    }
	std::cout << "\n" << "Press esc to exit" << std::endl;
     
    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        float CurrentTime = (float)glfwGetTime();
        DeltaTime = CurrentTime - LastFrame;    // 현재 프레임과 마지막 프레임 사이의 시간
        LastFrame = CurrentTime;
        float RealTime = (float)glfwGetTime();
        // input
        processInput(window);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);    //BG Color  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // depth buffer 초기화
         
        // ====================Reflected Cube======================
        LightingCube_Shader.use();
        LightingCube_Shader.setVec3("ObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        LightingCube_Shader.setVec3("LightColor",  SunLight);
        LightingCube_Shader.setVec3("SunPos", SunPos);
        // view, projection 생성
        glm::mat4 view = camera.ViewMatrix();  // View matrix(Dynamic Camera)
        glm::mat4 projection; // projection matrix : perspective 사용
        projection = glm::perspective(glm::radians(camera.CamFov()), (float)Screen_Width / (float)Screen_Height, 0.1f, 100.0f);
        // Vertex Shader로 전달
        LightingCube_Shader.setMat4("View", view);  // Shader Class 사용
        LightingCube_Shader.setMat4("Projection", projection);
        //---cube---
        glm::mat4 model = glm::mat4(1.0f); 
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, -3.0f));  
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        LightingCube_Shader.setMat4("Model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // ==========================Sun===========================
        SunLight_Shader.use();
        SunLight_Shader.setMat4("View", view);  // Vertex Shader로 전달  
        SunLight_Shader.setMat4("Projection", projection);
        //---Sun--- 
        model = glm::mat4(1.0f);
        glm::mat4 Sun = glm::translate(model, SunPos);
        Sun = glm::rotate(Sun, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        Sun = glm::rotate(Sun, RealTime * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  //자전축
        SunLight_Shader.setMat4("Model", Sun);

        glBindVertexArray(sunVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &sunVAO);
    glDeleteBuffers(1, &EBO); 
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
 
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
	// Mouse On/Off -> M | isMouseOn & isKeypressed = false
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        //이전 frame이 off & M키가 눌렸을 때만 진입
        if (!isKeypressed) {            
            isMouseOn = !isMouseOn;     
            if (isMouseOn) {            //이전 frame On & 현재 frame On
                glfwSetCursorPosCallback(window, mouse_Callback);   //마우스 카메라 활성화  
                glfwSetScrollCallback(window, scroll_callback);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //커서가 창의 중심에 유지(FPS)
            }
            else {
                glfwSetCursorPosCallback(window, NULL);     //마우스 카메라 비활성화
                glfwSetScrollCallback(window, NULL);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            isKeypressed = true; // M키가 눌려있는 동안 이전frame은 true
        }                        //설정을 키거나 끌 때 항상 키는 눌려있음
    }                          
    else {
        isKeypressed = false; //떼는 순간 false로 리셋
    }
    // WireFrame Mode  ->  w : line, F : fill
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
    // Camera Move(방향키)
    int keys[] = { GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL };
    for (int key : keys) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            camera.KeyboardControl(key, DeltaTime);
        }
    }
}

void mouse_Callback(GLFWwindow* window, double xPos, double yPos)
{
    camera.MouseControl((float)xPos, (float)yPos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.MouseScroll((float)yoffset);
}