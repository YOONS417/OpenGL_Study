#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>
#include "ShaderClass.h"
#include "stb_image.h"
#include "Camera.h"
#include "Texture.h"

extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_Callback(GLFWwindow* window, double xPos, double yPos);
void scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
void tutorial_light(Shader& LightingCube_Shader, const Camera& camera);
void multiplelight(Shader& Multiplelight_Shader, const Camera& camera, bool isFlashlightOn);

const unsigned int Screen_Width = 1200;
const unsigned int Screen_Height = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));   //카메라 생성, 위치:(0,0,10)

float DeltaTime = 0.0f , LastFrame = 0.0f; //카메라 이동 하드웨어 제한 방지(고정된 속도)
bool isMouseOn, isMpressed = false; // M키 설정
bool isFlashlightOn, isFpressed = false; // F키 설정
bool isWireframemodeOn, isWpressed = false;  // W키 설정
float NearPlane = 0.1f, FarPlane = 100.0f; // near, far plane

glm::vec3 Light_Direction(0.2f, -0.8f, 0.2f); // 평행광 방향(Directional Light)
glm::vec3 Pointlight_Pos(7.0f, 0.0f, 0.0f);   // Lighting cube 위치
glm::vec3 Terrain_Pos(0.0f, -5.0f, 0.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Screen_Width, Screen_Height, "Project_Depth&Stencil", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return  -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {std::cout << "GPU Vendor: "   << glGetString(GL_VENDOR)   << std::endl;
        std::cout << " Failed to initialze GLAD" << std::endl;
    }
	// Depth & Stencil Test
	glEnable(GL_DEPTH_TEST);   // 깊이 테스트 활성화
	glDepthFunc(GL_LESS);      // fragment의 깊이 값이 저장된 값보다 작을 경우만 통과
	glEnable(GL_STENCIL_TEST); // 스텐실 테스트 활성화
    // 같지 않을 때 테스트를 통과, 비교의 기중이 되는 숫자, 비교 전 값에 AND연산을 취할 마스크
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // 기본 Stencil 연산 정의 : stencil test 통과 시 stencil buffer에 1로 변경

    std::cout << "Current linkedGPU Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "=================Linked Shaders=================" << std::endl;
    Shader PointLight_Shader("Shaders/pointlight.vert", "Shaders/pointlight.frag");// 광원
    Shader WoodBox_Shader("Shaders/woodbox.vert", "Shaders/MultipleLight.frag");   // Cube Shader
    Shader Terrain_Shader("Shaders/Terrain.vert", "Shaders/Terrain.frag");         // Terrain Shader 
    Shader Outline_Shader("shaders/outline.vert", "Shaders/outline.frag");         // outline Shader

    float cube_vert[] = {  // each point : 0 ~ 7
        // Fornt surface      //법선                                                   index
       -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // left  bottom     = 0   0
        0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  // right  bottom    = 1   1   
        0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  // right  top       = 2   2
       -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // left  top        = 3   3
        // Right surface
        0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 1   4
        0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 5   5
        0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 6   6
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // left  top        = 2   7
        // Left surface
       -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 4   8
       -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 0   9
       -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 3  10
       -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // left  top        = 7  11
       // Top surface   
       -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 3  12
        0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 2  13
        0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 6  14
       -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // left  top        = 7  15
       // Bottom surface
       -0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 4  16
        0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 5  17
        0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 1  18
       -0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f,  // left  top        = 0  19
       // Back surface  
        0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,  // left  bottom     = 5  20
       -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 0.0f,  // right  bottom    = 4  21
       -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f,  // right  top       = 7  22
        0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f   // left  top        = 6  23
    };
    unsigned int cube_indices[] = {  // indices : 정점 데이터 배열의 행 번호(0~23)
         0, 1, 2,  0, 2, 3,       // Fornt surface
         4, 5, 6,  4, 6, 7,       // Right surface
         8, 9,10,  8,10,11,       // Left surface  
        12,13,14, 12,14,15,       // Top surface
        16,17,18, 16,18,19,       // Bottom surface       
        20,21,22, 20,22,23        // Back surface
    };
    float terrain[] = {       //normal            //texcoord
	   -1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // left  bottom     = 0
		1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // right  bottom    = 1
		1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // right  top       = 2
	   -1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f   // left  top        = 3
    };
    unsigned int terrain_indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    // cube
    unsigned int cubeVBO, cubeVAO, cubeEBO;
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO); // VAO에 바인딩 시작

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); //CPU 메로리에 있던 cube_vert를 GPU의 VBO메모리에 복사
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vert), cube_vert, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO); // VAO가 EBO를 기억아도록 바인딩
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // 각 면의 법선벡터
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //texture   
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // point light
    unsigned int sunVAO;
    glGenVertexArrays(1, &sunVAO);
    glBindVertexArray(sunVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // Terrain 
    unsigned int terrainVBO, terrainVAO, terrainEBO;
	glGenBuffers(1, &terrainVBO);
    glGenBuffers(1, &terrainEBO);
    glGenVertexArrays(1, &terrainVAO);

    glBindVertexArray(terrainVAO);

    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain), terrain, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(terrain_indices), terrain_indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
     
    // load texture & Lighting Maps
    std::cout << "=================Loaded Texture=================" << std::endl;
    unsigned int Cube_DiffuseMap = LoadTexture("Cube/woodbox.png");
    unsigned int Cube_SpecualrMap = LoadTexture("Cube/metaledge.png"); //specular image
    WoodBox_Shader.use();
    WoodBox_Shader.setInt("material.texture_diffuse1", 0);  //texture unit
    WoodBox_Shader.setInt("material.texture_specular1", 1); //빛의 세기를 조절하는 가이드라인으로만 사용s

	unsigned int Terrain_DiffuseMap = LoadTexture("Terrain/Rock058_2K-PNG_Color.png");
	unsigned int Terrain_SpecularMap = LoadTexture("Terrain/Rock058_2K-PNG_Roughness.png");
    Terrain_Shader.use();
	Terrain_Shader.setInt("material.texture_diffuse1", 0);  // 독립적인 shader이므로 0번부터 다시 사용 가능
	Terrain_Shader.setInt("material.texture_specular1", 1);

    // --Instruction-- 
    std::cout << "\n" << "=================Camera Control=================" << std::endl;
    std::string key[] = { "KEY_UP", "KEY_DOWN", "KEY_RIGHT", "KEY_LEFT", "SPACE_BAR", "CONTROL" ,"M", "Scroll" };
    std::string move[] = { "Forword", "Back", "Right", "Left", "Up" , "Down" ,"Mouse Camera On/Off", "Zoom in/out" };
    for (int i = 0; i < std::size(move); i++) {
        std::cout << key[i] << " : " << move[i] << std::endl;
    }
    std::cout << "\n" << "Press Esc to exit" << std::endl;

    /* 물체의 외곽선 그리기
        1. 오브젝트를 그리기 전에 stencil 함수를 GL_ALWAYS로 설정, 오브젝트의 fragment가 렌더링될때마다 stencil buffer를 1로 수정.
        2. 오브젝트를 렌더링
        3. stencil 작성과 depth testing을 비활성화.
        4. 각 오브젝트들을 약간 확대..
        5. 하나의 (외곽선)컬러를 출력하는 별도의 fragment shader를 사용.
        6. 오브젝트를 다시 그리지만 stencil 값이 1과 같지 않은 fragment들만 그리기.
        7. 다시 stencil 작성과 depth testing을 활성화.*/

    // --Render Loop-- 
    while (!glfwWindowShouldClose(window))  
    {
        float CurrentTime = (float)glfwGetTime();
        DeltaTime = CurrentTime - LastFrame;    // 현재 프레임과 마지막 프레임 사이의 시간
        LastFrame = CurrentTime;
        float RealTime = (float)glfwGetTime();
        
        processInput(window);   // input
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);    //BG Color  

        // Fragment -> Stencil -> Depth
		// stencil test를 통과한 fragment만 depth test를 진행 | 통과하지 못한 fragment는 버려지고 depth test연산 자체를 실행하지 않음
        // Stencil buffer 초기화 : 이전 프레임에세 남은 스텐실 값이 다음 프레임에 잔성처럼 영향 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

        // =============================Uniform shader==============================
        multiplelight(WoodBox_Shader, camera, isFlashlightOn);
		multiplelight(Terrain_Shader, camera, isFlashlightOn);

        // view, projection 생성    
        // projection matrix : perspective 사용        
        glm::mat4 view = camera.ViewMatrix();  // View matrix(Dynamic Camera)  
        glm::mat4 projection = glm::perspective(glm::radians(camera.CamFov()), (float)Screen_Width / (float)Screen_Height, NearPlane, FarPlane);
        // near가 0에 너무 가까우면 depth buffer의 정밀도가 떨어짐
        // far가 너무 멀면 이세한 z차이를 구분 못함(Z-fighting 발생)

        // =============================Terrain==============================
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glStencilMask(0x00);

        Terrain_Shader.use();
        Terrain_Shader.setMat4("View", view);
        Terrain_Shader.setMat4("Projection", projection);
        glm::mat4 terrain_model = glm::mat4(1.0f);
        terrain_model = glm::translate(terrain_model, Terrain_Pos);
        terrain_model = glm::scale(terrain_model, glm::vec3(50.0f, 1.0f, 50.0f));
        Terrain_Shader.setMat4("Model", terrain_model);
        // Bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Terrain_DiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Terrain_SpecularMap);
        // draw
        glBindVertexArray(terrainVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // =============================Wood Box============================== 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glStencilMask(0xFF);  // buffer 쓰기 열기
        // 상자가 그려지는 픽셀의 스텐실 값을 1로 기록
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // 0xFF : 8bit mask, 255(11111111)

        WoodBox_Shader.use();
        WoodBox_Shader.setMat4("View", view);
        WoodBox_Shader.setMat4("Projection", projection);

        glm::mat4 box_model = glm::mat4(1.0f);
        glm::vec3 box_scale = glm::vec3(2.0f, 2.0f, 2.0f);
        box_model = glm::scale(box_model, box_scale);
        WoodBox_Shader.setMat4("Model", box_model);
       
        // Bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Cube_DiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Cube_SpecualrMap);
        // draw
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // =============================Out Line==============================
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // 1이 아닌 영역(테두리)만 통과
        glStencilMask(0x00);        // 외곽선을 그리는 동안 stencil buffer 보호
        glDisable(GL_DEPTH_TEST);   // depth를 꺼서 terrain,poinlight에 외곽선이 묻힘 방지

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        Outline_Shader.use();
        Outline_Shader.setMat4("View", view);
        Outline_Shader.setMat4("Projection", projection);

        // --box1 outline--
        float outline_scale = 1.05;

        glm::mat4 model1_outline = glm::mat4(1.0f);
        model1_outline = glm::scale(model1_outline, box_scale * outline_scale);
        Outline_Shader.setMat4("Model", model1_outline);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glEnable(GL_DEPTH_TEST);


        // =============================Point Light==============================
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glStencilMask(0x00);    // pointlight,terrain은 스텐실 버퍼에 저장하지 않도록 잠금
        PointLight_Shader.use();
        PointLight_Shader.setMat4("View", view);  // Vertex Shader로 전달  
        PointLight_Shader.setMat4("Projection", projection);
        glm::mat4 pointligh_model = glm::mat4(1.0f);
        glm::mat4 Sun = glm::translate(pointligh_model, Pointlight_Pos);
        Sun = glm::rotate(Sun, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        Sun = glm::rotate(Sun, RealTime * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  //자전축
        Sun = glm::scale(Sun, glm::vec3(0.5f, 0.5f, 0.5f));
        PointLight_Shader.setMat4("Model", Sun);
        // draw
        glBindVertexArray(sunVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &sunVAO);
	glDeleteVertexArrays(1, &terrainVAO);
    glDeleteBuffers(1, &cubeEBO);
    glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &terrainEBO);

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
        Multiplelight_Shader.setFloat("spotlight.cutoff", glm::cos(glm::radians(7.0f))); //Spotlight의 반지름
        Multiplelight_Shader.setFloat("spotlight.outercutoff", glm::cos(glm::radians(10.0f))); //Spotlight의 부드러운 경계
    }
}