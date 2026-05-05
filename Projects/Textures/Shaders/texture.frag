#version 330 core

out vec4 FragColor;

in vec3 Shape_Color;
in vec2 Tex_Coord;

uniform sampler2D Shape_Texture;

void main() {							   // frag shader에서 텍스처에 정점 색을 곱하기
	FragColor = texture(Shape_Texture,Tex_Coord)* vec4(Shape_Color,1.0) ;
	if(FragColor.a < 0.1)  // 투명도가 낮은 곳은 아예 안 그리기 ( 배경없는 이미지 등 )
		discard;
	// texture 함수는 ( 샘플러 , 좌표) 인자가 필요
}
