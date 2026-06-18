#version 330 core
out vec4 FragColor;

in vec3 Normal;

uniform vec3 ObjectColor;
uniform vec3 LightColor;

void main() {
	//광원이 아닌 다른 곳에서 반사된 빛
	float ambientStrenght = 0.1f;  
	vec3 Ambient = ObjectColor * ambientStrenght;
	vec3 result = LightColor * Ambient;

	FragColor =  vec4(result , 1.0);
}