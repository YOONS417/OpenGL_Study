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
  
struct DirectionalLight {       // Sun
	//vec3 position;   // Sun Position | Directional light를 사용할 때는 필요 X
	vec3 direction;  // Directional Light : 광원으로부터 픽셀로 향하는 평행광 
	vec3 ambient;	 // (0.2, 0.2, 0.2)
	vec3 diffuse;	 // (0.5, 0.5, 0.5)
	vec3 specular;	 // (1.0, 1.0, 1.0)
};

uniform Material material;
uniform DirectionalLight light;  
uniform vec3 ViewPos;  //카메라 위치
  
void main() {      
	// Ambient light : 광원이 아닌 다른 곳에서 반사된 빛, 그늘진 곳이 완전히 까맟지 않고 희미하게 보임
	vec3 Ambient = light.ambient * texture(material.diffuse, TextureCoord).rgb;

	// Diffuse light : 광원의 빛이 직접 물체에 반사되는 빛(명암) 
	vec3 norm = normalize(NormalVector); // 법선 벡터를 정규화
	vec3 lightDirection = normalize(-light.direction); // 내적값을 위해 "픽셀->광원"을 요구, 크기 1유지
	float diff = max(dot(norm, lightDirection), 0.0); // max를 써서 음수 방지,  
	//diffuse가 0이 되더라도 최종결과에 ambient가 유지되어 실루엣이 남아 있음      
	vec3 Diffuse = light.diffuse * diff * texture(material.diffuse, TextureCoord).rgb;
  	// 광원 색 * 빛을 받는 각도 세기 * 픽셀 고유 색    

	// Specular light : 재질에 따른 눈부심(하이라이트)
	vec3 viewDir = normalize(ViewPos - FragPos); //(픽셀에서 카메라) 벡터를 정규화
	vec3 reflectDirection = reflect(-lightDirection, norm); //정반사된 단위벡터(light dierection은 픽셀에서 광원 벡터) 
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess); //pow : 거듭제곱 함수, 내적=cos값, max로 음수 방지(0=빛 없음) |32-> material.shininess
	// 0~1의 값을 거듭제곱(반사된 빛과 카메라의 사이각이 커질수록 수가 0의 수렴) 
	vec3 Specular = light.specular * spec * (texture(material.specular, TextureCoord).rgb * 2.0f);

	vec3 result = Ambient + Diffuse + Specular;  // 최종 색상 = ambient + diffuse + specular
	FragColor =  vec4(result , 1.0);  // 단 metalEdge 이미지의 안쪽이 검은색이라 specular계산 값이 0
 }    
// Specular(반사광)는 카메라 위치에 의존 : 빛이 튕기는 방향과 바라보는 방향이 일치할 때만
//  Diffuse(난반사,상자 윗면)가 안 밝은 이유 : 
