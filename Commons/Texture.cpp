#include <glad/glad.h>
#include <iostream>
#include "Texture.h"
#include "stb_image.h"

unsigned int LoadTexture(const std::string& path, const std::string &directory, bool flip) {
	std::string fullpath;
	if (directory.empty()) {
		fullpath = path;
	}
	else {
		if (directory.back() == '/' || directory.back() == '\\')
			fullpath = directory + path;
		else
			fullpath = directory + '/' + path;
	}

	unsigned int TextureID;
	glGenTextures(1, &TextureID);

	stbi_set_flip_vertically_on_load(flip);  // 이미지 상하 반전 설정

	int width, height, nrComponents;
	// CPU 메모리로 이미지 로드
	unsigned char* data = stbi_load(fullpath.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format = GL_RGB;
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;

		// GPU 메모리(VRAM)로 정송 및 바인딩
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// 텍스쳐 옵선 설정(repeat & filter) 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		std::cout << "Texture successfully loaded at path : " << fullpath << std::endl;
		std::cout << "Texture Image nrComponents : " << nrComponents << std::endl;
	}               
	else {
		std::cout << "Texture failed to load at path : " << fullpath << std::endl;
		stbi_image_free(data);
	}
	return TextureID;
}