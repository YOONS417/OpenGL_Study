#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderClass.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Project_Triangle", NULL, NULL);
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

    glm::vec4 vec(2.0f, 3.0f, 4.0f, 1.0f);    // vector 생성 , w=1 : 점 , w=0 : 방향
    glm::mat4 trans01 = glm::mat4(1.0f);   // 4x4 단위행렬 생성
    trans01 = glm::translate(trans01, glm::vec3(1.0f, 1.0f, 0.0f)); // translation 행렬에 이동 변환(1,1,0) 적용
    vec = trans01 * vec;
    std::cout << " ( " << vec.x << " , " << vec.y << " , " << vec.z << " )" << std::endl;


    glm::mat4 trans02 = glm::mat4(1.0f);
    trans02 = glm::rotate(trans02, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));  // z축 기준으로 90도 회전, 90도를 ㅠ/2로 바꿔서 계산
    trans02 = glm::scale(trans02, glm::vec3(0.5, 0.5, 0.5));
    
    Shader Transformation("Shaders/Trans.vert", "Shaders/Trans.frag");
   
    float vertices[] = {                        // texture coordinate
       -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // left bottom     =0
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // right bottom    =1
        0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // right  top      =2
       -0.5f, 0.5f, 0.0f,   0.0f, 0.7f, 0.7f    // left top        =3
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  6* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  6* sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

   

   // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window); 
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);   //BG Color
        glClear(GL_COLOR_BUFFER_BIT);

        Transformation.use();
        unsigned int trans_Location = glGetUniformLocation(Transformation.ID, "transform");
        glUniformMatrix4fv(trans_Location, 1, GL_FALSE, glm::value_ptr(trans02));

        glBindVertexArray(VAO);
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