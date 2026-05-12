#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "ShaderClass.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

const unsigned int Screen_Wdith = 800;
const unsigned int Screen_Height = 600;


int main() { 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Screen_Wdith, Screen_Height, "Project_Triangle", NULL, NULL);
    if (window == NULL) {  
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return  -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {   
        std::cout << " Failed to initialze GLAD" << std::endl;  
        glfwTerminate();
        return -1;
    } 
    
    Shader InOut_shader("Shaders/InOut.vert", "Shaders/InOut.frag");
    Shader Uniform_shader("Shaders/Uniform.vert", "Shaders/Uniform.frag");

    // -- Input Vertex Data -- 
    float InOut_vertices[] = { 
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,     // left + R
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,     // right + G
         0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f      // top  + B 
    };

    // vertices for uniform shader
    float Uniform_vertices[] = {
        -0.5f, -0.5f, 0.0f,      // left 
         0.5f, -0.5f, 0.0f,      // right 
         0.0f, 0.5f, 0.0f        // top  
    };

    unsigned int VBO, VAO  ;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(InOut_vertices), InOut_vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 3 -> 6
    glEnableVertexAttribArray(0);

    // Color attribute                nomalize = false / stride(6 : 점 3개, 색 3개)   /  offset(시작위치 : 4번째 부터)     
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

      
    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);   //BG Color
        glClear(GL_COLOR_BUFFER_BIT);

        
        // make color blink using uniform shader
        float TimeValue = glfwGetTime();  //실행시간으초 단위로
        float BlueValue = (sin(TimeValue * 1.5f) / 2.0f ) + 0.5f;  //sin으로 색을 변화 (0~1) , 1.5배
        int vertexColorLocation = glGetUniformLocation(Uniform_shader.ID, "OurColor"); //OurColor의 주소값을 찾아옴
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, BlueValue,  1.0f);  //찾은 주소에 BlueValue를 보냄 ,   glUseProgram(shaderProgram)을 반드시 호출 후에 실시
        // Frag shader의 OurColor에 전달  R ,   G  ,   B 
        

        //float offset = 0.3f;  // 각 꼭짓점의 offset을 이동
        //Uniform_shader.setFloat("Offset ", offset);  // vert shader의 Offset으로 값을 전달

        InOut_shader.use();
        //Uniform_shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } 
    glDeleteVertexArrays(1, &VAO);
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