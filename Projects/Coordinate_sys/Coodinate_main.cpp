#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderClass.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

const unsigned int Screen_Width = 1000;
const unsigned int Sereen_Height = 800;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Screen_Width, Sereen_Height, "Project_Coordinate_system", NULL, NULL);
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
     
    Shader Texture_Shader("Shaders/coord.vert", "Shaders/coord.frag");
   
    // vertex data
    float vertices[] = {                     
       -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // left bottom     =0
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // right bottom    =1
        0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,     // right  top      =2
       -0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f      // left top        =3
    };

    unsigned int indices[] = {
        0, 1, 2,   // right triangle
        0, 2 ,3    // left triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  8* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  8* sizeof(float), (void*)(3 * sizeof(float)));
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

    Texture_Shader.use();
    Texture_Shader.setInt("Tex_papersheet", 0);
    Texture_Shader.setInt("Tex_metalball", 1);

    glEnable(GL_DEPTH_TEST);

    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);    //BG Color  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);  // 0번 슬롯
        glBindTexture(GL_TEXTURE_2D, tex01);
        glActiveTexture(GL_TEXTURE1);  // 1번 슬롯
        glBindTexture(GL_TEXTURE_2D, tex02);

        Texture_Shader.use();
        glBindVertexArray(VAO);

        unsigned int trans_Location = glGetUniformLocation(Texture_Shader.ID, "transform");
        float scaleAmount = static_cast<float>( sin(glfwGetTime()) /3  ) +0.7;  // 색이 뒤집힘 없이
        float RealTime = (float)glfwGetTime();

        // 1st rectangle--Orbit01
        glm::mat4 i_mat = glm::mat4(1.0f);
        glm::mat4 trans03 = glm::rotate(i_mat, RealTime*0.9f, glm::vec3(0.0f, 0.0f, 1.0f));  // 공전
        trans03 = glm::translate(trans03, glm::vec3(0.75f, 0.0f, 0.0f));   // mat4에 translate 적용
        trans03 = glm::rotate(trans03, -RealTime *0.9f, glm::vec3(0.0f, 0.0f, 1.0f));  //공전의 반대로 미리 회전
        trans03 = glm::rotate(trans03, RealTime*5, glm::vec3(0.0f, 1.0f, 0.0f));  // 자전
        trans03 = glm::scale(trans03, glm::vec3(0.1f, 0.1f, 0.0f));
        glUniformMatrix4fv(trans_Location, 1, GL_FALSE, glm::value_ptr(trans03));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
          
        // 2nd rectangle--Orbit02
        glm::mat4 trans04 = glm::rotate(i_mat, -RealTime, glm::vec3(0.0f, 1.0f, 0.0f));  // 공전 , 축을 바라보는 방향이 반대 
        trans04 = glm::translate(trans04, glm::vec3(0.75f, 0.0f, 0.0f));
        trans04 = glm::rotate(trans04, RealTime*5, glm::vec3(0.0f, 1.0f, 0.0f));      // 자전
        trans04 = glm::scale(trans04, glm::vec3(0.1f, 0.1f, 0.0f));
        glUniformMatrix4fv(trans_Location, 1, GL_FALSE, glm::value_ptr(trans04));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // Middle rectangle
        glm::mat4 middle = glm::scale(i_mat, glm::vec3(scaleAmount, scaleAmount, 0.0f));
        middle = glm::scale(middle, glm::vec3(0.6f, 0.6f, 0.0f));
        glUniformMatrix4fv(trans_Location, 1, GL_FALSE, &middle[0][0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void proccessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}