#version 330 core
out vec4 FragColor;

in vec3 NormalVector;
in vec3 FragPos;
in vec2 TectureCoord;

void main () 
{
	FragColor = vec4(0.0f, 0.8f, 0.0f, 1.0f); // outline color

}