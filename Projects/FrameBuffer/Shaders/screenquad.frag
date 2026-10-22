#version 330
out vec4 FragColor;

in vec2 TextureCoord;

uniform sampler2D screenTexture;

void main ()
{
	FragColor = texture(screenTexture, TextureCoord);
}