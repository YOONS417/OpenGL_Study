
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ShaderClass.h"
#include "stb_image.h"
#include "Camera.h"
#include "Texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_Callback(GLFWwindow* window, double xPos, double yPos);
void scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
void tutorial_light(Shader& LightingCube_Shader, const Camera& camera);
void multiplelight(Shader& Multiplelight_Shader, const Camera& camera, bool isFlashlightOn);

const unsigned int Screen_Width = 1200;
const unsigned int Screen_Height = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));   //카메라 생성, 위치:(0,0,10)

float DeltaTime = 0.0f; //카메라 이동 하드웨어 제한 방지(고정된 속도)
float LastFrame = 0.0f;
bool isMouseOn, isMpressed = false; // M키 설정
bool isFlashlightOn, isFpressed = false; // F키 설정
bool isWireframemodeOn, isWpressed = false; //W키 설정

glm::vec3 SunPos(10.0f, 0.0f, 0.0f); //Sun position
glm::vec3 SunLightColor(1.0f, 1.0f, 1.0f);
glm::vec3 Light_Direction(0.2f, -0.8f, 0.2f); // 평행광 방향(Directional Light)
glm::vec3 Pointlight_Pos(7.0f, 0.0f, 0.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Screen_Width, Screen_Height, "Project_Model_Loading", NULL, NULL);
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

	// bulid & compile Shader program
    std::cout << "=================Linked Shaders=================" << std::endl;
    Shader LightingCube_Shader("Shaders/cube.vert", "Shaders/MultipleLight.frag");   //Cube Shader

    // load Models
    

 
    // --Instruction-- 
    std::cout << "\n" << "=================Camera Control=================" << std::endl;
    std::string key[] = { "KEY_UP", "KEY_DOWN", "KEY_RIGHT", "KEY_LEFT", "SPACE_BAR", "CONTROL" ,"M", "Scroll", "KEY_F", "KEY_W"};
    std::string move[] = { "Forword", "Back", "Right", "Left", "Up" , "Down" ,"Mouse Camera On/Off", "Zoom in/out", "Flash light", "Wireframe mode"};
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

        // ====================Light Reflected Cube======================
        //tutorial_light(LightingCube_Shader, camera);
        multiplelight(LightingCube_Shader, camera, isFlashlightOn);

        // view, projection 생성    
        glm::mat4 view = camera.ViewMatrix();  // View matrix(Dynamic Camera)  
        glm::mat4 projection; // projection matrix : perspective 사용
        projection = glm::perspective(glm::radians(camera.CamFov()), (float)Screen_Width / (float)Screen_Height, 0.1f, 100.0f);
        LightingCube_Shader.setMat4("View", view);  // Shader Class 사용, vertex shader로 전달
        LightingCube_Shader.setMat4("Projection", projection);
        //---cube---  
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, RealTime * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        LightingCube_Shader.setMat4("Model", model);
    
        // draw
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
   
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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
        if (!isMpressed) {
            isMouseOn = !isMouseOn;
            if (isMouseOn) {            //이전 frame On & 현재 frame On
                glfwSetCursorPosCallback(window, mouse_Callback);   //마우스 카메라 활성화  
                glfwSetScrollCallback(window, scroll_Callback);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //커서가 창의 중심에 유지(FPS)
            }
            else {
                glfwSetCursorPosCallback(window, NULL);     //마우스 카메라 비활성화
                glfwSetScrollCallback(window, NULL);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            isMpressed = true; // M키가 눌려있는 동안 이전frame은 true
        }                        //설정을 키거나 끌 때 항상 키는 눌려있음
    }
    else {
        isMpressed = false; //떼는 순간 false로 리셋
    }
    //Flash on/off
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) { 
        if (!isFpressed) {
            isFlashlightOn = !isFlashlightOn; //false -> true
            isFpressed = true;
        }
    }
    else {
        isFpressed = false;
    }
    //WireFramgeMode on/off
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { 
        if (!isWpressed) {  //true : on
            isWireframemodeOn = !isWireframemodeOn;
            if (isWireframemodeOn) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
              }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            isWpressed = true;
        }
    }
    else {
        isWpressed = false;
    }
    // Camera Move(방향키)
    int keys[] = { GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL };
    for (int key : keys) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            camera.KeyboardControl(key, DeltaTime);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.Rotate_Cam();
    }
}

void mouse_Callback(GLFWwindow* window, double xPos, double yPos)
{
    camera.MouseControl((float)xPos, (float)yPos);
}

void scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.MouseScroll((float)yoffset);
}

void tutorial_light(Shader& LightingCube_Shader, const Camera& camera)
{
    LightingCube_Shader.use();
    //LightingCube_Shader.setVec3("ObjectColor", glm::vec3(1.0f, 0.5f, 0.31f)); 
    //LightingCube_Shader.setVec3("LightColor",  SunLight);
    //LightingCube_Shader.setVec3("light.position", SunPos);       
    LightingCube_Shader.setVec3("light.position", camera.CamPosition);
    //LightingCube_Shader.setVec3("light.direction", Light_Direction); // 태양빛(평행빛)
    LightingCube_Shader.setVec3("light.direction", camera.CamFront);
    LightingCube_Shader.setFloat("light.cutoff", glm::cos(glm::radians(6.0f))); //Spotlight의 반지름 
    LightingCube_Shader.setFloat("light.outercutoff", glm::cos(glm::radians(9.0f))); //Spotlight의 부드러운 경계
    LightingCube_Shader.setVec3("ViewPos", camera.CamPosition);     //카메라 초기 위치
    // whtie light - basic setting | Distance setting : 100
    LightingCube_Shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));  //약한 주변광
    LightingCube_Shader.setVec3("light.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));  //직접광(중간 세기)
    LightingCube_Shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f)); //반사광(하이라이트)
    // Attenuation
    LightingCube_Shader.setFloat("light.constant", 1.0f);  // 일반적으로 상수항은 1
    LightingCube_Shader.setFloat("light.linear", 0.045f);
    LightingCube_Shader.setFloat("light.quadratic", 0.0075f);
    // Material
    //LightingCube_Shader.setVec3("material.ambient", glm::vec3(0.25f, 0.25f, 0.25f));
    //LightingCube_Shader.setVec3("material.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    //LightingCube_Shader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    LightingCube_Shader.setFloat("material.shininess", 64.0f);  // 하이라이트 조절
}

void multiplelight(Shader& Multiplelight_Shader, const Camera& camera, bool isFlashlightOn)
{
    Multiplelight_Shader.use();
    Multiplelight_Shader.setVec3("ViewPos", camera.CamPosition);
    Multiplelight_Shader.setFloat("material.shininess", 64.0f);  // 하이라이트 조절
    //Directional Light
    Multiplelight_Shader.setVec3("dirlight.direction", Light_Direction);
    Multiplelight_Shader.setVec3("dirlight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    Multiplelight_Shader.setVec3("dirlight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    Multiplelight_Shader.setVec3("dirlight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    //Point Light 
    Multiplelight_Shader.setVec3("pointlight.position", Pointlight_Pos);
    Multiplelight_Shader.setVec3("pointlight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    Multiplelight_Shader.setVec3("pointlight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    Multiplelight_Shader.setVec3("pointlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    Multiplelight_Shader.setFloat("pointlight.constant", 1.0f); //Distance setting(100)
    Multiplelight_Shader.setFloat("pointlight.linear", 0.045f);
    Multiplelight_Shader.setFloat("pointlight.quadratic", 0.0075f);
    //Spotlight
    Multiplelight_Shader.setBool("isFlashlightOn", isFlashlightOn);
    if (isFlashlightOn) {
        Multiplelight_Shader.setVec3("spotlight.position", camera.CamPosition);
        Multiplelight_Shader.setVec3("spotlight.direction", camera.CamFront);
        Multiplelight_Shader.setVec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        Multiplelight_Shader.setVec3("spotlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        Multiplelight_Shader.setVec3("spotlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        Multiplelight_Shader.setFloat("spotlight.constant", 1.0f); //Distance setting(100)
        Multiplelight_Shader.setFloat("spotlight.linear", 0.045f);
        Multiplelight_Shader.setFloat("spotlight.quadratic", 0.0075f);
        Multiplelight_Shader.setFloat("spotlight.cutoff", glm::cos(glm::radians(6.0f))); //Spotlight의 반지름
        Multiplelight_Shader.setFloat("spotlight.outercutoff", glm::cos(glm::radians(9.0f))); //Spotlight의 부드러운 경계
    }
}