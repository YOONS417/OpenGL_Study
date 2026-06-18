#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <iostream>

// --Setting--
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"  //가장 첫 줄(필수)
"layout (location = 0 ) in vec3 aPos;\n"                        
"void main() { gl_Position = vec4(aPos, 1.0); }\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() { FragColor = vec4(0.1f, 0.1f, 0.7f, 1.0f); }\0"; // 삼각형 색


int main() {     
    glfwInit();     //glfw : initialize & configure
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //V3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Project_Rectangle", NULL, NULL);    //glfw window creation
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

    // --Vertex shader--  
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);   
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  
    glCompileShader(vertexShader);      

    int success;        // check for shader compile errors
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << " ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);    
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); 
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);   
    if (!success) {     
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Shader program 
    unsigned int shaderProgram;     //link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);    
    glAttachShader(shaderProgram, fragmentShader);  
    glLinkProgram(shaderProgram);                 
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);    
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << " ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);  
    glDeleteShader(fragmentShader);

    // --vertex Input--  / Rectangle
    float vertices[] = {    //set NDC( Normalized Device Coordinates)
        -0.5f, -0.5f, 0.0f,     //left bottom   0
        0.5f, -0.5f, 0.0f,      //right bottom  1
        -0.5f, 0.5f, 0.0f,        //top left    2
        0.5f, 0.5f, 0.0f        //top right     3
    };

    unsigned int indices[] = {
        0, 1, 3,        // 첫번째 삼각형
        0, 3, 2         // 두번째 삼각형
    };

    unsigned int VBO, VAO, EBO;  
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &EBO);  //EBO생성 / 어떤 정점을 Vertex Shader에 보낼지
    glGenVertexArrays(1, &VAO);     

    glBindVertexArray(VAO);  // VAO는 glVertexAttribPointer() 호출 당시의 VBO 연결 상태를 기억함.
    //이제부터의 vertex 관련 설정을 이 VAO에 기록(바로 아래 6줄)
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // VBO와 유사하게 EBO는 인덱스들을 버퍼에 복사
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glVertexAttribPointer가Buffer 데이터와 Shader의 location을 연결한다 / 메모리 해석 규칙 설정!!
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //현재 bind된 VBO의 데이터를shader location 0에 연결
    glEnableVertexAttribArray(0);   
    //OpenGL은 최적화를 위해 정점 속성의 통로를 닫아둔 상태로 시작 -> 따로 명시 필요
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // ARRAY_BUFFER 슬롯 비우기
    glBindVertexArray(0);              // 현재 VAO 해제 ( unbind )

    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);
        glClearColor(0.8f, 0.7f, 0.2f, 1.0f); //배경색
        glClear(GL_COLOR_BUFFER_BIT);      // framebuffer 전체를 clear color로 덮기

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)   // WireFrame Mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // w : line, F : fill
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        glUseProgram(shaderProgram);   
        glBindVertexArray(VAO);         // 설정된 VAO를 다시 바인딩(그리기 전 필수)
        //glDrawArrays(GL_TRIANGLES, 0, 3);   //그리기 명령(도형 종류, 시작 인덱스, 정점 개수)
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    //모드,개수(index),타입,offset
        //  EBO 에서 제공되는 인덱스를 사용하여 그리게 됨

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // --cleanup--
    glDeleteVertexArrays(1, &VAO);     
    glDeleteBuffers(1, &VBO);  
    glDeleteBuffers(1, &EBO);
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