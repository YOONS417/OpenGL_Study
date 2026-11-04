#version 330
out vec4 FragColor;

in vec2 TextureCoord;

uniform sampler2D screenTexture;

void main ()
{
	FragColor = vec4(vec3(1.0 - texture(screenTexture, TextureCoord)), 1.0f);
}