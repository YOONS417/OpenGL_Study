#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <vector>
#include "ShaderClass.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;  // 정점 위치
	glm::vec3 Normal;	 // 법선	
	glm::vec2 TexCoord;	 //텍스쳐 좌표	
};

struct Texture {
	unsigned int id;    
	string type;	//diffuse, specular등 타입
};

class Meah {
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

}; 