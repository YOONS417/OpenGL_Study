#version 330 core   //diffuse map : 기본 바탕색 담당
out vec4 FragColor;

in vec3 NormalVector; // 큐브면의 법선 벡터
in vec3 FragPos;
in vec2 TextureCoord;

struct Material {  //ambient를 유지 -> 오브제트 전체에 동일하게 적용
	sampler2D diffuse;
	vec3 specular;  
	float shininess;     
};
  
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
      
uniform Material material;
uniform Light light;
uniform vec3 ViewPos;  //카메라 위치

void main() {      
	// Ambient light : 광원이 아닌 다른 곳에서 반사된 빛 
	float ambientStrenght = 0.1f;  	 
	vec3 Ambient = light.ambient * vec3(texture(material.diffuse, TextureCoord));

	// Diffuse light : 광원의 빛이 직접 물체에 반사되는 빛
	vec3 norm = normalize(NormalVector); // 법선 벡터를 정규화
	vec3 lightDirection = normalize(light.position - FragPos); // 면의 모든 픽셀마다 계산
	float diff = max(dot(norm, lightDirection), 0.0); //max를 써서 음수 방지 
	//diffuse가 0이 되더라도 ambient가 0.1로 유지되어 실루엣이 남아 있음      
	vec3 Diffuse =  light.diffuse * diff * vec3(texture(material.diffuse, TextureCoord));
  	// 광원 색 * 빛을 받는 각고 세기 * 픽셀 고유 색    

	// Specular light
	float specularStrenght = 0.5;
	vec3 viewDir = normalize(ViewPos - FragPos); //(픽셀에서 카메라) 벡터를 정규화
	vec3 reflectDirection = reflect(-lightDirection, norm); //정반사된 단위벡터(light dierection은 픽셀에서 광원 벡터) 
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess); //pow : 거듭제곱 함수, 내적=cos값, max로 음수 방지(0=빛 없음) |32-> material.shininess
	// 0~1의 값을 거듭제곱(반사된 빛과 카메라의 사이각이 커질수록 수가 0의 수렴) 
	vec3 Specular = light.specular * (spec *material.specular);
	   
	vec3 result = Ambient + Diffuse + Specular;  // 최종 색상 = ambient + diffuse + specular
	FragColor =  vec4(result , 1.0);
}   