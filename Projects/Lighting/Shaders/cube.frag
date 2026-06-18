#version 330 core
out vec4 FragColor;

uniform vec3 ObjectColor;
uniform vec3 LightColor;

float ambientStrenght = 0.1f;

void main() {
	vec3 Ambient = ObjectColor * ambientStrenght;
	vec3 result = LightColor * Ambient;

	FragColor =  vec4(result , 1.0);
}