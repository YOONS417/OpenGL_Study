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

// --Setting--
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"  //가장 첫 줄(필수)
    "layout (location = 0 ) in vec3 aPos;\n"            // VAO 설정 시 glVertexAttribPointer의 0번 인덱스와 이 변수를 연결              
    "void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }\0";
// 0번 입구로 들어오는 숫자(x,y,z)를 받아, 끝에 1.0을 붙여 4차원 좌료로 전화 후 gl_Position으로 전달
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() { FragColor = vec4(0.2f, 1.0f, 1.0f, 1.0f); }\0"; // 삼각형 색

int main() {
    glfwInit();     //glfw : initialize & configure
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //V3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Project_Triangle", NULL, NULL);    //glfw window creation
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return  -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glad : load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << " Failed to initialze GLAD" << std::endl;  
    }

    // --Vertex shader--  Compiling a shader/ 프로그램 가능한 shader
    unsigned int vertexShader;    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);    // shader 객체 생성, GL_VERTEX_SHADER : 점의 위치를 계산  => vertexShader에 생선된 객체의 ID가 저장
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  // shader객체에 실제 프로그래밍 코드 (GLSL)를 넣음
    glCompileShader(vertexShader);      // shader 코드를 기계어로 컴파일

    int success;        // check for shader compile errors
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << " ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog<<std::endl;
    }

    //Fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);    //객체 생성
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //색상 결정 로직의 문자열을 객체에 주입
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);   //shader가 에러 없이 컴파일되었는지 확인
    if (!success) {     //
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Shader program  ,  vertex shader와 fragment shader를 하나로 묶어서 실제로 렌더링하는 과정
    unsigned int shaderProgram;     //link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);    //shaderProgram에 컴파일된 셰이더들을 부착
    glAttachShader(shaderProgram, fragmentShader);  //프로그램 하나에느 반드시 vertex,fragment가 각각 하나씩 있어햐 함
    glLinkProgram(shaderProgram);                   //부착된 shader 둘을 서로 연결( vertex의 출력과 fragment의 입력이 서로 이름과 타입이 맞는지 확인 )
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);    //링크 과정이 성공했는지 확인
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << " ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);   //개별 객체를 유지할 필요가 없으므로 삭제
    glDeleteShader(fragmentShader); 

    // --vertex Input--  
    float vertices[] = {    //set NDC( Normalized Device Coordinates)
        -0.5f, -0.5f, 0.0f,     //left
        0.5f, -0.5f, 0.0f,      //right
        0.0f, 0.5f, 0.0f        //top
    };

    //VBO(실제 데이터) : cpu에서 GPU로 데이터를 매 프레임마다 하나씩 보내는 것은 매우 느림, VBO를 사용하여 데이터를 gpu메모리에 박아두고 필요할 떄 즉시 꺼내씀
    unsigned int VBO, VAO;  //vertex buffer object : gpu 메모리 내에 생성되는 버퍼, cup에 있는 정점데이터를 gpu로 한번에 전송
    glGenBuffers(1, &VBO); //버퍼 객체의 ID를 요청, 개수와 ID를 저장할 변수의 주소
    glGenVertexArrays(1, &VAO);     //VAO : 데이터를 어떻게 읽어야 하는지 정의하는 상태 저장 객체

    glBindVertexArray(VAO); //초기 설정, VBO와 속성 설정

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //ID를 가진 버퍼를 target에 연결, 용도와 바인딩할 버퍼의 ID
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //  cpu에 있던 vertices 데이터를 gpu 메모리로 복사,  GL_STATIC_DRAW : 데이터가 얼마나 자주 변경될지 힌트(static,dynamic,stream)
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //location = 0이어야 함 , size of vertex attribute(vec3), type, normalized = false, stride, Offset(시작점)
    glEnableVertexAttribArray(0);   //위에 방법대로 데이터가 흐를 수 있게 통로를 개방, 0은 location=0과 같게 설정
    //OpenGL은 최적화를 위해 정점 속성의 통로를 닫아둔 상태로 시작 -> 따로 명시 필요
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //Unbinding , 덮어씌워짐 방지
    // !!! VBO를 0으로 언바인딩헤도 VAO에 저장한 설정은 유지
    glBindVertexArray(0);     // 실수 방지, VAO를 해제하지 않고 다른 물체의 설정을 건드리면 내용이 덮어씌워 질 수 있음

    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);
        glClearColor(1.0f, 0.7f, 0.2f, 1.0f); //배경색
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);    //shaderProgram사용 선언
        glBindVertexArray(VAO);         // 설정된 VAO를 다시 바인딩(그리기 전 필수)
        glDrawArrays(GL_TRIANGLES, 0, 3);   //그리기 명령(도형 종류, 시작 인덱스, 정점 개수)

       // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // --cleanup--
    glDeleteVertexArrays(1, &VAO);      //VAO를 지운다고 VBO까지 자동으로 지워지지 않음
    glDeleteBuffers(1, &VBO);           //VBO를 비우고 GPU메모리를 반환
    glDeleteProgram(shaderProgram);     //셰이더 실행 파일 삭제
    //개별(vertex/fragment)을 지웠어도 program은 따로 지워야 완전히 사라짐


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