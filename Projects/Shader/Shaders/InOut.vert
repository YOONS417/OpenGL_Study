#version 330 core

layout ( location = 0 ) in vec3 aPos;   // 0 : 위치 , 1 : 색상
layout ( location = 1 ) in vec3 aColor; //cpu에서 VBO를 보낼 때, 몇 번 칸에 있는지 명시

out vec3 position;
out vec3 edge_Color;
//out vec4 vertexColor;		// output vertexColor to fragment shader

void main () {
	gl_Position = vec4(aPos.x , aPos.y, aPos.z, 1.0);		// 4dim homogencous coordinate로 변환 , gl_Position은 내장 변수
	position =  aPos;
	edge_Color = aColor;
	//vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0);
}
// *주석은 in out 변수이름이 vert, frag에서 vertexColor로 같은 경우*
// 타입과 변수이름이 같아 fragmentshader의 vertexColor이 vertexshader의 vertexColor와 자동으로 연결

/* ** gl_Position은 NDC(normalized device coordinate)로 변환( -1.0 ~ 1.0 )
	  한 꼭짓점의 좌표가 x,y 모두 음수 -> 색 X (검은색)
*/