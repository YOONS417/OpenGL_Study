#version 330 core
layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec3 aColor;
layout ( location = 2 ) in vec2 aTexCoordinate;

out vec3 Shape_Color;
out vec2 Texture_Coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	gl_Position =  Projection * View * Model * vec4(aPos, 1.0f);
	Shape_Color = aColor;
	Texture_Coord = aTexCoordinate;
}