#version 330 core

out vec4 FragColor;
uniform vec4 OurColor;	
// uniform은 전역 변수, vertexshader를 거칠 필요 X
// CPU에서 GPU shader로 데이터를 전달 , 코드에서 glUniform4f로 데이터를 넣어야 함
void main () {
	FragColor = OurColor; 
}