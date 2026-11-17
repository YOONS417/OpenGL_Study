#version 330
out vec4 FragColor;

in vec2 TextureCoord;

uniform sampler2D screenTexture;

uniform float xoffset;
uniform float yoffset;

void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-xoffset,  yoffset), // top left
		vec2(	 0.0f,  yoffset), // top center
		vec2( xoffset,  yoffset), // top right
		vec2(-xoffset,     0.0f), // center left
		vec2(	 0.0f,     0.0f), // center center
		vec2( xoffset,     0.0f), // center right
		vec2(-xoffset, -yoffset), // bottom left
		vec2(	 0.0f, -yoffset), // bottom center
		vec2( xoffset, -yoffset)  // bottom right    
	);
		
	float kernel[9] = float[] (
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	// 픽셀의 주변 8개의 픽셀을 가져오기
	vec3 sampleTexture[9];
	for(int i =0; i<9; i++)
	{
		sampleTexture[i] = vec3(texture(screenTexture, TextureCoord.st + offsets[i]));
		// 기본좌표 TextureCoord.st를 기준으로 주변 8개의 픽셀로 한칸씩 offset 만큼
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i<9; i++)
		col += sampleTexture[i] * kernel[i];  // 9개의 픽셀과 현재 픽셀을 kernel 값과 곱해서 더해주기	

	FragColor = vec4(col, 1.0f);
}
