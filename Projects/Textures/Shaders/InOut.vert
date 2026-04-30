#version 330 core

layout ( location = 0 ) in vec3 aPos;   // 0 : 위치 , 1 : 색상
layout ( location = 1 ) in vec3 aColor; //cpu에서 VBO를 보낼 때, 몇 번 칸에 있는지 명시

out vec3 edge_Color;
//out vec4 vertexColor;		// output vertexColor to fragment shader

void main () {
	gl_Position = vec4(aPos, 1.0);		// 4dim homogencous cooordinate로 변환 , gl_Position은 내장 변수
	edge_Color = aColor;
	//vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0);
}

// 타입과 변수이름이 같아 fragmentshader의 vertexColor이 vertexshader의 vertexColor와 자동으로 연결