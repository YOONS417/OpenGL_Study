#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

unsigned int LoadTesture(const std::string& path, bool flip) {
	unsigned int TextureID;
	glGenTextures(1, &TextureID);

	stbi_set_flip_vertically_on_load(flip);

	int width, height, nrComponents;

	unsigned char* data = stbi_load(path.c_str(), &width, &height, & nrComponents, 0);
	
	if (data) {
		GLenum format = GL_RGB;
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;


	}   
}