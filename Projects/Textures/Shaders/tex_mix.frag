#version 330 core
out vec4 FragColor;

in vec3 Shape_Color;
in vec2 Tex_Coord;

uniform sampler2D Tex_paper;
uniform sampler2D Tex_metal;

void main() {
	vec4 Paper = texture(Tex_paper, Tex_Coord);
	vec4 MetalBall = texture(Tex_metal, Tex_Coord);
	FragColor = mix(Paper, MetalBall, MetalBall.a*0.75); // Tex_Color02.a : Alpha 채널
	/*FragColor = mix( texture(Tex_paper, Tex_Coord), texture(Tex_metal, Tex_Coord), 0.7); //이미지를 섞은 후 투명도 설정 
	if(FragColor.a < 0.1)  // 투명도가 낮은 곳은 아예 안 그리기 ( 배경없는 이미지 등 )
		discard; */
}
