#include <iostream>
#include <fstream>
#include < sstream>
#include  <string>
#include "ShaderLoader.h"

std::string loadShaderSource(const char* filePath) { //읽어올 파일의 경로를 받고, 모든 텍스트들을 하나의 긴 문자열로 변환
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);  // 파일을 읽기 모드로 열기

    if (!fileStream.is_open()) {
        std::cerr << " File could not be found " << filePath << std::endl;
        return "";
    }
    std::stringstream sstr; //데이터를 임시로 저장
    sstr << fileStream.rdbuf();  // 파일 전체 내용 한번에 가져오기
    content = sstr.str();
    fileStream.close();
    return content;
} 