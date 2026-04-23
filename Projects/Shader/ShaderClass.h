#ifndef SHADER_H //  중복으로 정의되어 방샐하는 컴파일 에러 방지
#define SHADER_H 

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
public:
    unsigned  int ID;   // shader prog id(주소)

    Shader(const char* vertexPath, const char* fragmentPath)   // read .vert .frag & compile
    {                                                          // link two shader into one shader prog
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << " ERROR::SHADER::FILE_NOT_SUCCESFULLY READ" << e.what() <<  std::endl;
        }

        const char* vShaerCode = vertexCode.c_str();  // warning : 
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int VertexShader, FragmentShdaer;
        // --Vertex Shader--
        VertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShader, 1, &vShaerCode, NULL);
        glCompileShader(VertexShader);  
        CheckCompileError(VertexShader, "Vertex");
        // --Fragment Shader--
        FragmentShdaer = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShdaer, 1, &fShaderCode, NULL);
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

    void use() {         // 내부적으로 glUserProgram(ID)를 실행(활성화)
        glUseProgram(ID);
    }

    // uniform 유틸리티
    //void setBool(const std::string &name, bool value) const;   
    //void setInt(const std::string& name, int value) const;
    //void setFloat(const std::string& name, float value) const;

private:
    void CheckCompileError(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                 glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                 std::cout << " ERROR::SHADER_COMPILATION_ERROE of type : " << type<< "\n" << infoLog << std::endl;
             }
        }
        else {
            glGetShaderiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog (shader, 1024, NULL, infoLog);
                std::cout << " ERROR::SHADER::LINKING_ERROR of type ; \n" << type << "\n" << infoLog << std::endl;
            }
        }
    }
};
#endif