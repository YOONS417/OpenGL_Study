#version 330 core
out vec4 FragColor;
//in vec4 vertexColor;	// same vertexColor in vertexShaderSource
in vec4 edge_Color;
void main() {
	//FragColor = vertexColor;
	FragColor = edge_Color;
}
// 타입과 변수이름이 같아 fragmentshader의 vertexColor이 vertexshader의 vertexColor와 자동으로 연결
