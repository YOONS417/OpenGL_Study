#version 330 core
layout( location = 0 ) in vec3 aPos;
layout( location = 1 ) in vec3 aColor;
layout( location = 2 ) in vec2 aTexCoord;

out vec3 Shape_Color;
out vec2 Tex_Coord;

void main() 
{
	gl_Position = vec4(aPos, 1.0);
	Shape_Color = aColor;
	Tex_Coord = aTexCoord;
}	 