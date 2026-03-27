#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <iostream>
//삼각형 그리기 
/*   --실제 코드를 작석하는 부분은 2,5--
    1. vertex data : 삼각형의 세 꼭짓점 좌표를 정의
    2. vertex shader : 각 꼭짓점의 위치를 처리(3D 좌표를 화면의 2D좌표로 바꾸는계산)
    3. primirive assembly : 점들을 연결해 삼각형 현태를 만듦
    4. rasterization : 수학적 삼감형을 화면상의 픽셀(fragment)로 쪼갬
    5. fragment shader : 각 픽셀의 색상을 결정
    6. blending/testing : 깊이 테스트나 투명도 처리를 거쳐 최종 화면에 출력     */

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
    "layout (lacation = 0 ) in vec3 aPos;\n"
    "void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\0";
const char* fragmentShaderSource = "version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() { FragColor = vec4{ 0.3f, 0.4f, 0.7f, 1.0f); } \0"; 


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

    // --Vertex shader--  Compiling a shader/ 프로그램 가능한 shader
    unsigned int vertexShader;    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;        // chsek fot shader compile errors
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << " ERROR::SHADER::VERTEX::OCMPILATION_FAILED\n" << infoLog<<std::endl;
    }

    //Fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER""FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(vertexShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << " ERROR::SHADER::PROGRAM::OCMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

    // --vertex Input--
    float vertices[] = {    //set NDC( Normalized Device Coordinates)
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
    };

    unsigned int VBO;  //vertex buffer object : gpu 메모리 내에 생성되는 버퍼, cup에 잇는 데이터를 gpu로 한번에 전송
    glGenBuffers(1, &VBO); //버퍼 객체의 ID를 요청, 개수와 ID를 저장할 변수의 주소
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //ID를 가진 버퍼를 target에 연결, 용도와 바인딩할 버퍼의 ID
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //  cpu에 있던 vertices 데이터를 gpu 메모리로 복사,  GL_STATIC_DRAW : 데이터가 얼마나 자주 변경될지 힌트(static,dynamic,stream)
    

   


   

    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);
        glClearColor(0.1f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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