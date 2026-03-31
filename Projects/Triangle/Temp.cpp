/*#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 1. 셰이더 코드 (GPU에서 실행되는 작은 프로그램)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() { FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); }\0"; // 주황색

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // 2. 삼각형 정점 데이터 (NDC 좌표: -1.0 ~ 1.0)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 왼쪽 아래
         0.5f, -0.5f, 0.0f, // 오른쪽 아래
         0.0f,  0.5f, 0.0f  // 위쪽 중앙
    };

    // 3. GPU 메모리에 데이터 보내기 (VAO, VBO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 4. 셰이더 컴파일 (이 부분은 보통 함수로 빼지만 일단 메인에 넣었습니다)
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 5. 렌더링 루프
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 드디어 그리기!

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}*/