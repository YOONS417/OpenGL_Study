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

		unsigned int VAO, VBO, EBO;  //버퍼 객체

		// 생성자 : 데이터 할당 및 GPU 버퍼 세팅
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}

		void Draw(Shader& shader)
		{

		}
	private:
		void setupMesh()
		{
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			// VBO : Vertex 구조체 배열을 GPU 메모리에 복사
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
			// EBO : 인덱스 데이터 복사
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// 정점 위치
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// 각 면의 법선
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		}
}; 