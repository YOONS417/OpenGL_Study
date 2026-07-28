#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include "ShaderClass.h"
#include "stb_image.h"
#include "Camera.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_Callback(GLFWwindow* window, double xPos,double yPos);
void scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int Screen_Width = 1200;
const unsigned int Screen_Height = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));   //카메라 생성, 위치:(0,0,10)

float DeltaTime = 0.0f; //카메라 이동 하드웨어 제한 방지(고정된 속도)
float LastFrame = 0.0f; 
bool isMouseOn, isKeypressed = false; // M키 설정

glm::vec3 SunPos(0.0f, 1.0f, 0.0f); //Sun position
glm::vec3 SunLight(1.0f, 1.0f, 1.0f);
glm::vec3 Light_Direction(0.2f, -0.8f, -0.3f); // 평행광 방향

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
    Shader LightingCube_Shader("Shaders/cube.vert", "Shaders/Light_casters.frag");     //Cube Shader

    float cube_vert[] = {  // each point : 0 ~ 7
         // Fornt surface      //법선 
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // left  bottom     = 0
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  // right  bottom    = 1
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  // right  top       = 2
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // left  top        = 3
         // Right surface
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 1
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 5
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 6
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // left  top        = 2
         // Left surface
        -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 4
        -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 0
        -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 3
        -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // left  top        = 7
         // Top surface   
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 3
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 2
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 6
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left  top        = 7
         // Bottom surface
        -0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 0
         0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 1
         0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 5
        -0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f,  // left  top        = 4
         // Back surface
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,  // left  bottom     = 4
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 0.0f,  // right  bottom    = 5
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f,  // right  top       = 6
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f   // left  top        = 7
    };
    unsigned int cube_indices[] = {  // indices : 정점 데이터 배열의 행 번호(0~23)
        0, 1,  2,  0, 2, 3,       // Fornt surface
        4, 5,  6,  4, 6, 7,       // Right surface
        8, 9, 10,  8,10,11,       // Left surface  
        12,13,14, 12,14,15,       // Top surface
        16,17,18, 16,18,19,       // Bottom surface    
        20,21,22, 20,22,23        //  Back surface
    };
    float terrain[] = {
         -1.0f, -1.0f,  1.0f, 
          1.0f, -1.0f,  1.0f,
          1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f
    };
    unsigned int terrain_indices[] = {
        0, 3, 2,
        0, 1, 2
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 각 면의 볍선벡터
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)) );
    glEnableVertexAttribArray(1);
    //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    unsigned int sunVAO;
    glGenVertexArrays(1, &sunVAO); 
    glBindVertexArray(sunVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    // load texture & Lighting Maps
    unsigned int DiffuseMap = LoadTexture("woodbox.png"); 
    unsigned int SpecualrMap = LoadTexture("metaledge.png"); //specular image
    LightingCube_Shader.use();
    LightingCube_Shader.setInt("material.diffuse", 0);  //texture unit
    LightingCube_Shader.setInt("material.specular", 1); //빛의 세기를 조절하는 가이드라인으로만 사용s

	// --Instruction-- 
	std::cout << "\n" << "================Camera Control================" << std::endl;
    std::string key[] = { "KEY_UP", "KEY_DOWN", "KEY_RIGHT", "KEY_LEFT", "SPACE_BAR", "CONTROL" ,"M", "Scroll"};
    std::string move[] = { "Forword", "Back", "Right", "Left", "Up" , "Down" ,"Mouse Camera On/Off", "Zoom in/out"};
    for (int i = 0; i < std::size(move); i++) {
        std::cout << key[i] << " : " << move[i] << std::endl;
    } 
	std::cout << "\n" << "Press esc to exit" << std::endl;
    // Random Position Cubes with different angle
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-5.0f, 5.0f);
    std::uniform_real_distribution<float> disY(-5.0f, 5.0f);
    std::uniform_real_distribution<float> disZ(-5.0f, 5.0f);
    const int Cube_count = 5;
    std::vector<glm::vec3> randomPos;
    for (int i=0; i < Cube_count; i++) {
        randomPos.push_back(glm::vec3(disX(gen), disY(gen), disZ(gen)));
    }
    
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
        LightingCube_Shader.use();
        //LightingCube_Shader.setVec3("ObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        //LightingCube_Shader.setVec3("LightColor",  SunLight);
        //LightingCube_Shader.setVec3("light.position", SunPos);        
        LightingCube_Shader.setVec3("light.direction", Light_Direction); // 태양빛(평행빛)
        LightingCube_Shader.setVec3("ViewPos", camera.CamPosition); 
        // whtie light - basic setting
        LightingCube_Shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));  //약한 주변광
        LightingCube_Shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));  //직접광(중간 세기)
        LightingCube_Shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f)); //반사광(하이라이트)

        //LightingCube_Shader.setVec3("material.ambient", glm::vec3(0.25f, 0.25f, 0.25f));
        //LightingCube_Shader.setVec3("material.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        //LightingCube_Shader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        LightingCube_Shader.setFloat("material.shininess", 64.0f);  // 하이라이트 조절
        
        // view, projection 생성    
        glm::mat4 view = camera.ViewMatrix();  // View matrix(Dynamic Camera)  
        glm::mat4 projection; // projection matrix : perspective 사용
        projection = glm::perspective(glm::radians(camera.CamFov()), (float)Screen_Width / (float)Screen_Height, 0.1f, 100.0f);
        // Vertex Shader로 전달
        LightingCube_Shader.setMat4("View", view);  // Shader Class 사용
        LightingCube_Shader.setMat4("Projection", projection);
        //---cube---  
        glm::mat4 model = glm::mat4(1.0f);     
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, -5.0f));  
        model = glm::rotate(model, RealTime * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        LightingCube_Shader.setMat4("Model", model);   
        // Bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, DiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, SpecualrMap);
        // draw
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 

        // ===========================Random Cube========================
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < Cube_count; i++) {
            model = glm::mat4(1.0f);
            float angle = 20.0f * i;
            model = glm::translate(model, randomPos[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
            LightingCube_Shader.setMat4("Model", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        } 
        // =============================Sun==============================
        /*SunLight_Shader.use();
        SunLight_Shader.setMat4("View", view);  // Vertex Shader로 전달  
        SunLight_Shader.setMat4("Projection", projection);
        //---Sun--- 
        model = glm::mat4(1.0f);
        glm::mat4 Sun = glm::translate(model, SunPos);
        Sun = glm::rotate(Sun, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        Sun = glm::rotate(Sun, RealTime * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  //자전축
        Sun = glm::scale(Sun, glm::vec3(0.5f, 0.5f, 0.5f)); 
        SunLight_Shader.setMat4("Model", Sun);
        // draw
        glBindVertexArray(sunVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        */
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
                glfwSetScrollCallback(window, scroll_Callback);
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