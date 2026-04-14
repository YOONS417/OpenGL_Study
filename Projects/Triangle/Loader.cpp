#include <fstream>
#include<sstream>
#include<iostream>
#include<string>

std::string loadShaderSource(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << " File could not be found" << filePath << std::endl;
		return "";
	}

	std::stringstream sstr;
	sstr << fileStream.rdbuf();
	content = sstr.str();
	fileStream.close();
	return content;
}