#version 330 core
out vec4 FragColor;

in vec3 Shape_Color;
in vec2 Tex_Coord;

uniform sampler2D Tex_paper;
uniform sampler2D Tex_metal;

void main() {
	vec4 Paper = texture(Tex_paper, Tex_Coord);
	vec4 MetalBall = texture(Tex_metal, Tex_Coord);
	FragColor = mix(Paper, MetalBall, MetalBall.a*0.8); // Tex_Color02.a : Alpha 채널
	/*FragColor = mix( texture(Tex_paper, Tex_Coord), texture(Tex_metal, Tex_Coord), 0.7); //이미지를 섞은 후 투명도 설정 
	if(FragColor.a < 0.1)  // 투명도가 낮은 곳은 아예 안 그리기 ( 배경없는 이미지 등 )
		discard; */
}
/*
구분,					mix (함수),						BLEND (기능)
개념,					색의 합성,						레이어의 혼합
수식,				(A * (1-t)) + (B * t),				(소스 * 알파) + (데스티네이션 * (1-알파))
비유,			팔레트 위에서 물감을 섞기,				이미 칠해진 도화지 위에 셀로판지 겹치기
필요 상황,	하나의 물체에 여러 텍스처를 입힐 때,		여러 물체가 서로 투명하게 겹쳐 보일 때 */	