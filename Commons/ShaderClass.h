#ifndef SHADER_H  //  linking conflicts 방지 
#define SHADER_H  //  중복으로 정의되어 발생하는 컴파일 에러 방지

#include <glad/glad.h>
#include <iostream> 
#include <fstream>   
#include <sstream>
#include <string> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader {  
public:
    unsigned  int ID;   // Shader prog id(주소)
                     // "Shaders/InOut.vert", "Shaders/InOut.frag"
    Shader(const char* vertexPath, const char* fragmentPath)   // read .vert .frag & compile
    {                                                          // link two shader into one shader prog
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;  // 파일을 읽기위한 도구역할( ifstream 객체)
        std::ifstream fShaderFile;
        // C++의 파일 읽기는 기본적으로 파일이 없거나 오류가 나도 멈추지 않고 무시
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);   // exceptions으로 무시하지 않고 에러를 알림
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {       
            vShaderFile.open(vertexPath);   // Find & Open file  
            fShaderFile.open(fragmentPath);

            std::stringstream vShaderStream, fShaderStream; // temp

            vShaderStream << vShaderFile.rdbuf();  // rdbuf() : 파일 내용 전체를 메모리에 저장 / 속도가 매우 빠름
            fShaderStream << fShaderFile.rdbuf(); 

            vShaderFile.close(); 
            fShaderFile.close();

            vertexCode = vShaderStream.str();    // 텍스트를 C++에서 다루기 쉬운 std:string 형태로 변환 -> vertexCode에 저장  
            fragmentCode = fShaderStream.str();  // str() -> C++(std::string)
        }
        catch (std::ifstream::failure e) {  // try에서 에러 -> catch
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ  : " << e.what() <<  std::endl;
        }                                                          // e.what() : 구체적인 에러 원인 출력
        // --OpenGL은 C언어의 라이브러리--
        const char* vertexShaderCode = vertexCode.c_str();   // C++ -> C / 문자열 포인터로 변환
        const char* fragmentShaderSource = fragmentCode.c_str();

        unsigned int VertexShader, FragmentShdaer;
        // --Vertex Shader--
        VertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(VertexShader);  
        CheckCompileError(VertexShader, "Vertex"); // ID, string
        // --Fragment Shader--
        FragmentShdaer = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShdaer, 1, &fragmentShaderSource, NULL);
        glCompileShader(FragmentShdaer);
        CheckCompileError(FragmentShdaer, "Fragment");
        // --Shader Program--
        ID = glCreateProgram();
        glAttachShader(ID, VertexShader);  
        glAttachShader(ID, FragmentShdaer);
        glLinkProgram(ID);
        CheckCompileError(ID, "PROGRAM");   

        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShdaer);
    }
    // --use(activate) shader--
    void use() {         // 내부적으로 glUserProgram(ID)를 실행(활성화)
        glUseProgram(ID);
    }

    // --utility Uniform--          glGetUniformLocation(ID, name.c_str()) -> shader id(위치), uniform변수의 이름(OurColor) 
    void setBool(const std::string& name, bool value) const { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);  
    }
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()) , value);
    }
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

private:
    void CheckCompileError(unsigned int shader, std::string type) { // ID, "Vertex/Fragment/PROGRAM"
        int success;
        char infoLog[512];

        if (type != "PROGRAM")  // Vertex & Fragment
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);  // 컴파일되었는지 상태 검사
            if (!success) {
                 glGetShaderInfoLog(shader, 512, NULL, infoLog);
                 std::cout << "ERROR::SHADER_COMPILATION_ERROE of type : " << type << "\n" << infoLog << "\n=================================================" << std::endl;
             }
        }
        else { // PROGRAM
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog (shader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::LINKING_ERROR of type : " << type << "\n" << infoLog << "\n=================================================" << std::endl;
            }
        }
    }
};
#endif