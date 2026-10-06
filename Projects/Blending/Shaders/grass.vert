#version 330 core
layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TextureCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection; 

void main() {
	FragPos = vec3(Model * vec4(aPos,1.0f));
	// 큐브 정점의 실제(월드)위치를 계산 후 전달
	// non-uniform scale과 rotation을 방지하기 위한 Normal matrix로 적용
	TextureCoord = aTexCoords;
	gl_Position =  Projection * View * Model * vec4(aPos, 1.0f);
}