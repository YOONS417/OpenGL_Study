#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPos;

void main() {
	// Ambient light : 광원이 아닌 다른 곳에서 반사된 빛
	float ambientStrenght = 0.1f;   	
	vec3 Ambient = ObjectColor * ambientStrenght; 
	// diffuse light : 광원의 빛이 직접 물체에 비치는 빛
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDirection), 0.0); 
	vec3  Diffuse = diff* LightColor;

	vec3 result = (Ambient + Diffuse) * ObjectColor;

	FragColor =  vec4(result , 1.0);
}