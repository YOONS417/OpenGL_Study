#version 330 core
out vec4 FragColor;

in vec3 Shape_Color;
in vec2 Texture_Coord;

uniform sampler2D Tex_papersheet;
uniform sampler2D Tex_metalball;

void main() {
	vec4 Paper =texture(Tex_papersheet, Texture_Coord);
	vec4 Metalball = texture(Tex_metalball, Texture_Coord)*vec4(Shape_Color,1.0f);
	FragColor = mix(Paper,Metalball, Metalball.a *0.8 );
}