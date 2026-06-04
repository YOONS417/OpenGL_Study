#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderClass.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int Screen_Width = 1200;
const unsigned int Screen_Height = 900;

glm::vec3 CamPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CamUp = glm::vec3(0.0f, 1.0f, 0.0f);

float DeltaTime = 0.0f; //하드웨어 제한 방지(고정된 속도)
float LastFrame = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Screen_Width, Screen_Height, "Project_Coordinate_system", NULL, NULL);
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

    Shader Cube_Shader("Shaders/cube.vert", "Shaders/cube.frag");


    float cube_vert[] = {  // each point : 0 ~ 7
         // Fornt surface
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,      // left  bottom     = 0
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,      // right  bottom    = 1
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,      // right  top       = 2
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,      // left  top        = 3
         // Right surface
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // left  bottom     = 1
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // right  bottom    = 5
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,     // right  top       = 6
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,     // left  top        = 2
         // Left surface
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // left  bottom     = 4
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // right  bottom    = 0
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,     // right  top       = 3
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,     // left  top        = 7
         // Top surface
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // left  bottom     = 3
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // right  bottom    = 2
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,     // right  top       = 6
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,     // left  top        = 7
         // Bottom surface
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // left  bottom     = 0
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // right  bottom    = 1
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,     // right  top       = 5
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,     // left  top        = 4
         // Back surface
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // left  bottom     = 4
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // right  bottom    = 5
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,     // right  top       = 6
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,     // left  top        = 7
    };

    unsigned int cube_indices[] = {  // indices : 정점 데이터 배열의 행 번호(0~23)
        0, 1,  2,  0, 2, 3,       // Fornt surface
        4, 5,  6,  4, 6, 7,       // Right surface
        8, 9, 10,  8,10,11,       // Left surface  
        12,13,14, 12,14,15,       //Top surface
        16,17,18, 16,18,19,       //Bottom surface    
        20,21,22, 20,22,23        //Back surface
    };
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vert), cube_vert, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int tex01, tex02;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    // Paper Texture
    glGenTextures(1, &tex01);
    glBindTexture(GL_TEXTURE_2D, tex01);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load("PaperSheet.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Papaer Image nrChannel : " << nrChannels << std::endl;
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Metal ball
    glGenTextures(1, &tex02);
    glBindTexture(GL_TEXTURE_2D, tex02);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("MetalBall.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "MetalBall image nrChannel : " << nrChannels << std::endl;
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    Cube_Shader.use();
    Cube_Shader.setInt("Tex_papersheet", 0);
    Cube_Shader.setInt("Tex_metalball", 1);
    //setInt는 반드시 shader가 켜져있을 때만 작동 , 내부적으로 glUniform1i라는 함수를 호출
    //유니폼 변수에 값을 넣으려면 반드시 그 유니폼을 가지고 있는 shaderprogrma이 켜져 있는 상태

    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        float CurrentTime = (float)glfwGetTime();
        DeltaTime = CurrentTime - LastFrame;
        LastFrame = CurrentTime;

        float RealTime = (float)glfwGetTime();

        processInput(window);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);    //BG Color  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // depth buffer 초기화

        glActiveTexture(GL_TEXTURE0);  // 0번 슬롯,paper
        glBindTexture(GL_TEXTURE_2D, tex01);
        glActiveTexture(GL_TEXTURE1);  // 1번 슬롯,ball
        glBindTexture(GL_TEXTURE_2D, tex02);

        Cube_Shader.use();
        glBindVertexArray(VAO);

        
        //===Camera(동적 카메라) 원리=========================================================================
        //카메라 뒤(+z방향)를 가리키는 벡터 구하기, 기본적으로 -Z방향을 봄
        glm::vec3 camera_Position = glm::vec3(0.0f, 0.0f, 8.0f);
        glm::vec3 camera_Target = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 camera_Direction = glm::normalize(camera_Position - camera_Target); // normalize(정규화) -> 크기를 1로 맟춤
        // +X방향 벡터
        glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 camera_Right = glm::normalize(glm::cross(up_vector, camera_Direction));
        // +Y방향 벡터
        glm::vec3 cameraUp = glm::cross(camera_Direction, camera_Right);
        glm::mat4 view_mat = glm::lookAt(camera_Position, camera_Target, cameraUp);  //cameraPos, cameraTarget, cameraUp
        //====================================================================================================
        
        // View matrix(Dynamic Camera)
        glm::mat4 view = glm::lookAt(CamPos, CamPos + CamFront, CamUp); //두번째 인자를 고정 점(원점)인 아닌 -Z방향 주시

        // projection matrix : perspective 사용
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)Screen_Width / (float)Screen_Height, 0.1f, 100.0f);

        // send view,projection matrix to shader
        Cube_Shader.setMat4("View",view );  // Shader Class 사용
        Cube_Shader.setMat4("Projection", projection);

        //---Sun--- 
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 Sun = glm::rotate(model, RealTime * glm::radians(45.0f), glm::vec3(-1.0f, 1.7f, 0.3f)); //vec()을 축으로 1초에 60도씩 회전
        Sun = glm::rotate(Sun, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));  //자전축

        Cube_Shader.setMat4("Model", Sun);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //---Earth--- 
        glm::mat4 earth = glm::rotate(model, RealTime * glm::radians(90.0f), glm::vec3(1.0f, 3.0f, 1.0f)); //1초에 60도 공전
        earth = glm::translate(earth, glm::vec3(0.0f, 0.0f, 4.0f));    //공전 반지름 
        earth = glm::rotate(earth, glm::radians(-23.5f), glm::vec3(0.0f, 0.0f, 1.0f)); //자전축 기울기
        earth = glm::rotate(earth, RealTime * glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //1초에 120도 자전
        earth = glm::scale(earth, glm::vec3(0.4f, 0.4f, 0.4f));

        Cube_Shader.setMat4("Model", earth);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //---Moon---
        glm::mat4 moon = earth;   //지구의 움직임을 상속
        moon = glm::rotate(moon, RealTime * glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //공전
        moon = glm::translate(moon, glm::vec3(2.5f, 0.0f, 0.0f));   //공전 반지름
        moon = glm::rotate(moon, RealTime * glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));  //자전
        moon = glm::scale(moon, glm::vec3(0.5f, 0.5f, 0.5f));

        Cube_Shader.setMat4("Model", moon);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
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

    // WireFrame Mode  ->  w : line, F : fill
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Camera Move
    const float CameraSpeed = 2.0f * DeltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        CamPos += CameraSpeed * CamFront;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        CamPos -= CameraSpeed * CamFront;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        CamPos += glm::normalize(glm::cross(CamFront, CamUp)) * CameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        CamPos -= glm::normalize(glm::cross(CamFront, CamUp)) * CameraSpeed;
    
}

// pos = pos - speed*front;