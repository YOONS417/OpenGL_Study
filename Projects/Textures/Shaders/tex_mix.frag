#version 330 core
out vec4 FragColor;

in vec3 Shape_Color;
in vec2 Tex_Coord;

uniform sampler2D Tex_paper;
uniform sampler2D Tex_metal;

void main() {
	FregColor = mix( texture(Tex_paper, Tex_Coord), texture(Tex_metal, Tex_Coord), 0.2);
}
