#version 330 core

layout ( location = 0 ) in vec3 aPos;

uniform float Offset;

void main () {
	gl_Position = vec4(aPos.x + Offset, aPos.y, aPos.z , 1.0);	//gl_Position은 내장 변수
}