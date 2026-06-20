#version 330 core
out vec4 FragColor;

in vec3 NormalVector;
in vec3 FragPos;

uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 SunPos;

void main() {
	// Ambient light : 광원이 아닌 다른 곳에서 반사된 빛
	float ambientStrenght = 0.1f;   	
	vec3 Ambient = ObjectColor * ambientStrenght; 
	// diffuse light : 광원의 빛이 직접 물체에 비치는 빛
	vec3 norm = normalize(NormalVector); // 법선 벡터를 정규화
	vec3 lightDirection = normalize(SunPos - FragPos); // 면의 모든 픽셀마다 계산
	float diff = max(dot(norm, lightDirection), 0.0); //max를 써서 0이하는 내려가지 않게
	vec3  Diffuse = diff* LightColor;
	//diffuse가 0이 되더라도 ambient가 0.1로 유지되어 실루엣이 남아 있음
	vec3 result = (Ambient + Diffuse) * ObjectColor;
	FragColor =  vec4(result , 1.0);
}