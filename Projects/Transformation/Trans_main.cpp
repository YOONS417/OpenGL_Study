#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderClass.h"

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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


    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);    // ( 1, 0, 0, 1) 생성 , w=1 : 점 , w=0 : 방향
    glm::mat4 translation = glm::mat4(1.0f);   // 4x4 단위행렬 생성
    translation = glm::translate(translation, glm::vec3(1.0f, 1.0f, 0.0f)); // translation 행렬에 이동 변환(1,1,0) 적용

    vec = translation * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;



    Shader Translation("Trans.vert", "Trans.frag");
   
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

        Translation.use();
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