#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "ShaderLoader.h"

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
    // --include headerfile & separate class / call GLSL 
    std::string vertexCode = loadShaderSource("Shaders/InOut.vert");   // read file from shader.vert 
    std::string fragmnetCode = loadShaderSource("Shaders/InOut.frag");     // read file from shader.frag
    std::string  vUniformCode= loadShaderSource("Shaders/Uniform.vert");
    std::string fUniformCode= loadShaderSource("Shaders/Uniform.frag");

    const char* vSahderSource = vertexCode.c_str();
    const char* fShaderSource = fragmnetCode.c_str();
    const char* v_uniformSource = vUniformCode.c_str();
    const char* f_uniformSource = fUniformCode.c_str();

    // --Vertex Shader--
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v_uniformSource, NULL);  //sourcecode from file
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << " ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // --FragmentShader--
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f_uniformSource, NULL);  //sourcecode from file
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << " ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // --ShaderProgram--
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout <<" ERROR::SHADER::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog <<std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // -- Input Vertex Data -- 
    float vertices[] = { 
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,     //left + R
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,      //right + G
         0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f       //top  + B 
    };

    unsigned int VBO, VAO  ;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position atttibute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 3 -> 6
    glEnableVertexAttribArray(0);

    // color attribute                nomalize = false / stride(6 : 점 3개, 색 3개)   /  offset(시작위치 : 4번째 부터)     
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);
        glClearColor(0.1f, 0.2f, 0.6f, 1.0f);   //BG Color
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // 
        float TimeValue = glfwGetTime();  //실행시간으초 단위로
        float GreenValue = (sin(TimeValue * 1.5f) / 2.0f ) + 0.5f;  //sin으로 색을 변화 (0~1)
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "OurColor"); //OurColor의 주소값을 찾아옴
        glUniform4f(vertexColorLocation, 0.0f, GreenValue, 0.0f, 1.0f);  //찾은 주소에 GreenValue를 보냄 ,   glUseProgram(shaderProgram)을 반드시 호출 후에 실시

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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