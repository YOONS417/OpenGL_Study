#version 330 core

out vec4 FragColor;

in vec3 Shape_Color;

void main() {
	FragColor = vec4(Shape_Color, 1.0);
}