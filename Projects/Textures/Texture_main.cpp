#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "ShaderClass.h"
#include "stb_image.h"


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

    Shader TEXTURE_Shader("Shaders/texture.vert", "Shaders/texture.frag"); 

    // -- Input Vertex Data -- 
    float vertices[] = {                        // texture coordinate
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // left bottom     =0
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // right bottom    =1
         0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // right  top      =2
        -0.5f, 0.5f, 0.0f,   0.0f, 0.7f, 0.7f,  0.0f, 1.0f  // left top        =3
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2 ,3
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

    // --Position attribute--
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 3 -> 8
    glEnableVertexAttribArray(0);
    // --Color attribute--        * nomalize = false / stride(6 : 점 3개, 색 3개)   /  offset(시작위치 : 4번째 부터)     
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // --Texture Coordinate arrtibute--
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --Generating Texture--
    unsigned int texture;
    glGenTextures(1, &texture);   // ( 생성할 텍스처의 개수 , 텍스처 ID 
    glBindTexture(GL_TEXTURE_2D, texture);  // 바인 딩해야 이후의 텍스처 관련 명령어들이 현재 바인딩된 텍스처를 설정
    // texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // 이미지를 상하 반전해서 로드
    unsigned char* data = stbi_load("PaperSheet.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // 텍스처 유형 , 밉맵 레벨 , 이미지를 어떤 포멧으로 저장할지 결정(RGB), border(항상 0), data가 어떤 구성으로 되었는지, 데이터 타입 , 실제 이미지 픽셀정보가 담긴 주소)
        glGenerateMipmap(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // --Render Loop--
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);   //BG Color
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)   // WireFrame Mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // w : line, F : fill
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        glBindTexture(GL_TEXTURE_2D, texture);

        TEXTURE_Shader.use();
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