#version 330 core

out vec4 FragColor;
//in vec4 vertexColor;	// same vertexColor in vertexShaderSource
in vec3 edge_Color;

void main() {
	//FragColor = vertexColor;
	FragColor = vec4(edge_Color, 1.0);
}
// 타입과 변수이름이 같아 fragmentshader의 vertexColor이 vertexshader의 vertexColor와 자동으로 연결
