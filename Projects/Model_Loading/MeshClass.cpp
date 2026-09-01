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

		unsigned int VAO;  //버퍼 객체

		// 생성자 : 데이터 할당 및 GPU 버퍼 세팅
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}

		void Draw(Shader& shader)
		{
			unsigned int diffuse_nr = 1;	// 텍스쳐 타입의 개수
			unsigned int specular_nr = 1;

			for (unsigned int i = 0; i < textures.size(); i++)  // 텍스쳐의 총 개수까지
			{
				glActiveTexture(GL_TEXTURE0 + i);	// 다음 텍스쳐 슬롯을 계속 활성화

				// 하나의 메쉬에 여러 개의 diffuse나 specular가 포함될 수 있음
				string number;
				string name = textures[i].type;
				// uniform이름을 동적으로 만듬 , ex) material.texture_diffuse1
				if (name == "texture_diffuse")
					number = to_string(diffuse_nr++);
				else if (name == "texture_specular")
					number = to_string(specular_nr++);
				// shader의 material.texture_diffuse1 유니폼 변수가 몇 번째 텍스쳐 슬록을 참조하는지 알림
				shader.setInt(("material." + name + number).c_str(), i); 
				// GPU의 i번째 텍스쳐 슬롯에 텍스쳐 ID를 바인딩
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
			// Mesh 그리기
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
			// 상태 복원
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);
		}

	private:
		unsigned int VBO, EBO;
		void setupMesh()
		{
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			glGenVertexArrays(1, &VAO);

			glBindVertexArray(VAO);

			// VBO : Vertex 구조체 배열을 GPU 메모리에 복사
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
			// EBO : 인덱스 데이터 복사
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// 정점 위치
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			// 각 면의 법선 
			// 구조체는 멤버가 메모리에 연속적으로 배치 -> offsetof : Normal 변수가 몇 바이트 떨어져 있는지 자동로 계산
			// (void*)(3 * sizeof(float)) - > (void*)offsetof(Vertex, Normal)
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			// 텍스쳐 좌표
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

			glBindVertexArray(0);
		}
}; 