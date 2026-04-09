#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

const unsigned int Screen_Wdith = 800;
const unsigned int Screen_Height = 600;

const char* vertexShaderSource = "#version 330 core\n" //vec4 type vertexColor
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;"         //vertexColor 전달
"void main() { gl_Position = vec4(aPos, 1.0);\n "   //gl_Position은 내장 변수
"vertexColor = vec4(0.5, 0.0, 0.0, 1.0); }";
// 타입과 변수이름이 같아 fragmentshader의 vertexColor이 vertexshader의 vertexColor와 자동으로 연결
const char* fragmentShaderSource = "#version 330 core\n"    // smae vertexColor in vertexShaderSource
"out vec4 FragColor;\n"                                    
"in vec4 vertexColor; \n"
"void main () { FragColor = vertexColor; } \n";

   
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
    // --Vertex Shader--
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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

    // -- Input Vertex Data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,        //left
        0.5f, -0.5f, 0.0f,         //right
        0.0f, 0.5f, 0.0f           //top 
    };

    unsigned int VBO, VAO  ;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);
        glClearColor(0.1f, 0.1f, 0.4f, 1.0f);   //BG Color
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
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