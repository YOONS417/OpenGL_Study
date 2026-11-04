#version 330
out vec4 FragColor;

in vec2 TextureCoord;

uniform sampler2D screenTexture;

void main ()
{
	FragColor = texture(screenTexture, TextureCoord);
	float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0f;
	float avg_weighted =  0.21 * FragColor.r + 0.7 * FragColor.g + 0.2 * FragColor.b;
	FragColor = vec4(avg_weighted, avg_weighted, avg_weighted, 1.0f);	
}