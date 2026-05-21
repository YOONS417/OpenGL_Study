#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderClass.h"

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

    GLFWwindow* window = glfwCreateWindow(Screen_Width, Sereen_Height, "Project_Transformation", NULL, NULL);
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

    // translate vec
    glm::vec4 vec(2.0f, 3.0f, 4.0f, 1.0f);    // vector 생성 , w=1 : 점 , w=0 : 방향
    glm::mat4 trans01 = glm::mat4(1.0f);   // 4x4 단위행렬 생성

    trans01 = glm::translate(trans01, glm::vec3(1.0f, 1.0f, 0.0f)); // translation 행렬에 이동 변환(1,1,0) 적용
    vec = trans01 * vec;
    std::cout << " ( " << vec.x << " , " << vec.y << " , " << vec.z << " )" << std::endl;
    
    // rotate & scale vec  
    glm::mat4 trans02 = glm::mat4(1.0f);    //  회전각도           회전 축     
    trans02 = glm::rotate(trans02, glm::radians(45.0f), glm::vec3(1.0, 1.0, 0.0));  // 축 기준으로 회전, radians(90.f) :  90도를 ㅠ/2로 바꿔서 계산
    trans02 = glm::scale(trans02, glm::vec3(0.5, 0.5, 0.5));    // scaling -> x 0.5

    
    Shader Transformation("Shaders/Trans.vert", "Shaders/Trans.frag");
   
    float vertices[] = {                        // texture coordinate
       -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // left bottom     =0
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // right bottom    =1
        0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // right  top      =2
       -0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 1.0f    // left top        =3
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
        glBindVertexArray(VAO);

        unsigned int trans_Location = glGetUniformLocation(Transformation.ID, "transform");

        // 1st rectangle
        glm::mat4 i_mat = glm::mat4(1.0f);
        glm::mat4 trans03 = glm::rotate(i_mat, (1/2)*(float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        trans03 = glm::translate(trans03, glm::vec3(0.5f, -0.5f, 0.0f));   // mat4에 translate 적용
        trans03 = glm::rotate(trans03, (float)glfwGetTime()*2, glm::vec3(0.0f, 0.0f, 1.0f));  //z축 기준으로 매순간 회전, 2배속
        trans03 = glm::scale(trans03, glm::vec3(0.4f, 0.4f, 0.0f));
        glUniformMatrix4fv(trans_Location, 1, GL_FALSE, glm::value_ptr(trans03));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // 2nd rectangle(orbit)
        glm::mat4 trans04 = glm::rotate(i_mat, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, .0f));
        trans04 = glm::scale(trans04, glm::vec3(0.6f, 0.6f, 0.0f));
        glUniformMatrix4fv(trans_Location, 1, GL_FALSE, glm::value_ptr(trans04));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // 3rd rectangle
        glm::mat4 trans05 = glm::translate(i_mat, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleAmount = static_cast<float>(sin(glfwGetTime())/2 +0.5);  // 색이 뒤집힘 없이
        trans05 = glm::scale(trans05, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        glUniformMatrix4fv(trans_Location, 1, GL_FALSE, &trans05[0][0]);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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