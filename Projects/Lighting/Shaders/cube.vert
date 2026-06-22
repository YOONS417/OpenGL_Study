#version 330 core
layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec3 aNormal; 

out vec3 FragPos;
out vec3 NormalVector; //물체표면의 법선
 
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection; 

void main() {
	gl_Position =  Projection * View * Model * vec4(aPos, 1.0f);
	// 큐브 정점의 실제(월드)위치를 계산 후 전달
	FragPos = vec3(Model*vec4(aPos,1.0f));
	NormalVector = mat3(transpose(inverse(Model))) * aNormal;
	// non-uniform scale과 rotation을 방지하기 위한 Normal matrix로 적용
}