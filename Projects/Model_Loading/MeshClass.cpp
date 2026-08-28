#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include "ShaderClass.h"

using namespace std;

//데이터 보관 : 정점, 인덱스, 텍스쳐
//GPU 버퍼 관리 : VAO, VBO, EBO 생성 및 바인딩
struct Vertex {
	glm::vec3 Position;  // 정점 위치
	glm::vec3 Normal;	 // 법선	
	glm::vec2 TexCoord;	 // 텍스쳐 좌표	
};

struct Texture {
	unsigned int id;	// 텍스쳐 바인딩 ID   
	string type;		// diffuse, specular등 타입
	string path;		// 파일 경로(중복 로드 방지)
};

class Mesh {
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		unsigned int VAO;

		// 생성자 : 데이터 할당 및 GPU 버퍼 세팅
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}

	private:
		void setupMesh()
		{

		}
}; 