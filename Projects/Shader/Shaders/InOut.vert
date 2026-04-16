#version 330 core

layout ( location = 0 ) in vec3 aPos;
out vec4 vertexColor;		//vertexColor 전달
void main () {
	gl_Position = vec4(aPos, 1.0);		//gl_Position은 내장 변수
	vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0);
}
// 타입과 변수이름이 같아 fragmentshader의 vertexColor이 vertexshader의 vertexColor와 자동으로 연결
