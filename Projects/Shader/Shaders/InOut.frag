#version 330 core

out vec4 FragColor;
//in vec4 vertexColor;	// same vertexColor in vertexShaderSource
in vec3 edge_Color;
in vec3 position;

void main() {  
	//FragColor = vertexColor;
	vec3 temp = (position+1.0)/ 2.0;
	FragColor = vec4(temp, 1.0);
}
// 타입과 변수이름이 같아 fragmentshader의 vertexColor이 vertexshader의 vertexColor와 자동으로 연결
/*  절댓값 : abs() -> 대칭형

*/