#version 330 core
out vec4 FragColor;
//in vec4 vertexColor;	// same vertexColor in vertexShaderSource
in vec4 edgeColor;
void main() {
	//FragColor = vertexColor;
	FragColor = edgeColor;
}
// 타입과 변수이름이 같아 fragmentshader의 vertexColor이 vertexshader의 vertexColor와 자동으로 연결
