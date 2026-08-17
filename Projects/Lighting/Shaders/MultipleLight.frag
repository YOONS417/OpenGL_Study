#version 330 core   //diffuse map : 기본 바탕색 담당
out vec4 FragColor;

in vec3 NormalVector; // 큐브면의 법선 벡터
in vec3 FragPos;
in vec2 TextureCoord;

struct Material {  //ambient를 유지 -> 오브제트 전체에 동일하게 적용
	sampler2D diffuse;	 // Diffuse map, 0번 슬롯	 
	sampler2D specular;  // Specular map, 1번 슬롯
	float shininess;	 // 64	
};
// Directional Light 
struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
// Point Light
struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	// Distance setting : 50
	float constant;
	float linear;
	float quadratic;
};
// Spotlight
struct SpotLight {       // Point Light 
	vec3 position;   // Directional light를 사용할 때는 필요 X
	vec3 direction;  // Directional Light : 광원으로부터 픽셀로 향하는 평행광 
	vec3 ambient;	 // (0.2, 0.2, 0.2)
	vec3 diffuse;	 // (0.6, 0.6, 0.6)
	vec3 specular;	 // (1.0, 1.0, 1.0)
	// Distance setting : 50  
	float constant;	  // Kc
	float linear;	  // Kl
	float quadratic;  // Kq
	float cutoff;     // 빛이 도달할 수 있는 최대 범위를 지정
	float outercutoff;
	bool isFlashlightOn;
};

uniform Material material;
uniform DirectionalLight dirlight;
uniform PointLight pointlight;
uniform SpotLight spotlight;  
uniform vec3 ViewPos;  //카메라 위치
 
vec3 CalculateDirLight(DirectionalLight light, vec3 NormalVector, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 NormalVector, vec3 FragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 NormalVector, vec3 FragPos, vec3 viewDir);

void main() {      
	vec3 norm = normalize(NormalVector);
	vec3 viewDir = normalize(ViewPos - FragPos); // (픽셀->카메라)벡터

	vec3 result = CalculateDirLight(dirlight, norm, viewDir):
}

vec3 CalculateDirLight(DirectionalLight light, vec3 NormalVector, vec3 viewDir){
	vec3 LightDirection = normalize(-light.direction);// 내적값을 위해 "픽셀->광원"을 요구, 크기 1유지
	float diff = max(dot(NormalVector, LightDirection), 0.0);// max를 써서 음수 방지
	vec3 reflectDirection = reflect(-LightDirection, NormalVector);//정반사된 단위벡터(light dierection은 픽셀에서 광원 벡터) 
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);//pow : 거듭제곱 함수, 내적=cos값, max로 음수 방지(0=빛 없음) |32-> material.shininess
	// 0~1의 값을 거듭제곱(반사된 빛과 카메라의 사이각이 커질수록 수가 0의 수렴) 
	vec3 Ambient = light.ambient * vec3(texture(material.diffuse, TextureCoord)); // 기본 밝기 * 텍스처 색상(픽셀 고유 색)
	vec3 Diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoord));// 광원 색 * 빛을 받는 각도 세기 * 텍스처 색상
	vec3 Specular = light.specular * spec * vec3(texture(material.specular, TextureCoord));// 하이라이트 빛 색상 *  시선 방향에 따른 반사 강도 * 어느 부위가 얼마나 반짝이는지 지어하는 텍스처
	return Ambient + Diffuse + Specular;
}
	
vec3 CalculatePointLight(PointLight light, vec3 NormalVector, vec3 FragPos, vec3 viewDir){
	vec3 LightDirection = normalize(light.position - FragPos);//(픽셀 -> 빛)방향
	float diff = max(dot(NormalVector, LightDirection), 0.0);
	vec3 reflectDirection = reflect(-LightDirection, NormalVector);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0 ), material.shininess);
	float distance = length(light.position - FragPos);//빛과 픽셀 사이의 거리
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// 거리별 빛의 세기 감소(attenuation)  
	vec3 Ambient = light.ambient * vec3(texture(material.diffuse, TextureCoord)); // 기본 밝기 * 텍스처 색상(픽셀 고유 색)
	vec3 Diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoord));// 광원 색 * 빛을 받는 각도 세기 * 텍스처 색상
	vec3 Specular = light.specular * spec * vec3(texture(material.specular, TextureCoord));// 하이라이트 빛 색상 *  시선 방향에 따른 반사 강도 * 어느 부위가 얼마나 반짝이는지 지어하는 텍스처
	Ambient *= attenuation;
	Diffuse *= attenuation;
	Specular *= attenuation;
	return Ambient + Diffuse + Specular;
}

vec3 CalculateSpotLight(SpotLight light, vec3 NormalVector, vec3 FragPos, vec3 viewDir){
	vec3 LightDirection = normalize(light.position - FragPos);//(픽셀 -> 빛)방향
	float diff = max(dot(NormalVector, LightDirection), 0.0);
	vec3 reflectDirection = reflect(-LightDirection, NormalVector);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0 ), material.shininess);
	float distance = length(light.position - FragPos);//빛과 픽셀 사이의 거리
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// 거리별 빛의 세기 감소(attenuation), 출발점을 통일하기 위해 -light.direction : 픽셀 -> 광원
	float theta = dot(LightDirection, normalize(-light.direction));//각 픽셀에서 SpotDir과 lightDirection사이의 각도
	float epsilon = (light.cutoff - light.outercutoff); // 전이 영역 : 내부(cutoff)와 외부(outercutoff)사이의 공간
	float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);// 1~0으로 제한, 전이 영역에서 빛이 감소
	vec3 Ambient = light.ambient * vec3(texture(material.diffuse, TextureCoord)); // 기본 밝기 * 텍스처 색상(픽셀 고유 색)
	vec3 Diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoord));// 광원 색 * 빛을 받는 각도 세기 * 텍스처 색상
	vec3 Specular = light.specular * spec * vec3(texture(material.specular, TextureCoord));// 하이라이트 빛 색상 *  시선 방향에 따른 반사 강도 * 어느 부위가 얼마나 반짝이는지 지어하는 텍스처
	Ambient *= attenuation  * intensity;
	Diffuse *= attenuation * intensity;
	Specular *= attenuation * intensity;
	return Ambient + Diffuse + Specular;
}