#version 330 core
out vec4 FragColor;

in vec3 NormalVector; // 큐브면의 법선 벡터
in vec3 FragPos;

uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 SunPos; // loop에서 물체와 광원 사이의 거리를 위해 광원 위치를 받음
uniform vec3 ViewPos;  //카메라 위치

void main() {
	float ambientStrenght = 0.1f; // Ambient light : 광원이 아닌 다른 곳에서 반사된 빛   	
	vec3 Ambient = ObjectColor * ambientStrenght; 
	// diffuse light : 광원의 빛이 직접 물체에 반사되는 빛
	vec3 norm = normalize(NormalVector); // 법선 벡터를 정규화
	vec3 lightDirection = normalize(SunPos - FragPos); // 면의 모든 픽셀마다 계산
	float diff = max(dot(norm, lightDirection), 0.0); //max를 써서 음수 방지 
	vec3  Diffuse = diff  * LightColor;
	//diffuse가 0이 되더라도 ambient가 0.1로 유지되어 실루엣이 남아 있음

	float specularStrenght = 0.5;
	vec3 viewDir = normalize(ViewPos - FragPos); //(픽셀에서 카메라) 벡터를 정규화
	vec3 reflectDirection = reflect(-lightDirection, norm); //정반사된 단위벡터(light dierection은 픽셀에서 광원 벡터) 
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0), 64); //pow : 거듭제곱 함수, 내적=cos값, max로 음수 방지(0=빛 없음)
	// 0~1의 값을 거듭제곱(반사된 빛과 카메라의 사이각이 커질수록 수가 0의 수렴) 
	vec3 Specular = specularStrenght * spec * LightColor;

	vec3 result = (Ambient + Diffuse + Specular) * ObjectColor;
	FragColor =  vec4(result , 1.0);
}  