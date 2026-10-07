#version 330 core
layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

out vec2 TextureCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection; 

void main() {
	TextureCoord = aTexCoords;	
	gl_Position =  Projection * View * Model * vec4(aPos, 1.0f);
}