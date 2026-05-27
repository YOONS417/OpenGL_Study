#version 330 core
layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec3 aColor;
layout ( location = 2 ) in vec2 aTexCoordinate;

out vec3 Shape_Color;
out vec2 Texture_Coord;

uniform mat4 transform;

void main() {
	gl_Position =  transform*vec4(aPos, 1.0);
	Shape_Color = aColor;
	Texture_Coord = aTexCoordinate;
}