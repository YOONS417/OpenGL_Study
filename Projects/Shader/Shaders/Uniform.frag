#version 330 core

out vec4 FragColor;
uniform vec4 OurColor;	
//uniform은 전역 변수, vertexshader를 거칠 필요 X
void main () {
	FragColor = OurColor;
}