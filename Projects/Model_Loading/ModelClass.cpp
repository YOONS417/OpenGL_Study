#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "MeshClass.h"

using namespace std;

// 텍스쳐 파일 로드용 함수
unsigned  int TextureFromFile(const char* path, const std::string& direcroty);

class Model
{
	public:
		vector<Mesh> meshes;
		string direcroty;

		vector<Texture> loaded_texture;

		Model(string const& path)
		{
			loadModel(path);
		}
		// 반복해서 mesh들의 Draw함수를 호출
		void Draw(Shader shader)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(shader);
		}

	private:
		// 파일 경로를 받아 Assimp를 통해 파일 내부의 3D데이터 구조를 GPU가 처리할 수 있는 형태로 로딩
		void loadModel(string const& path)
		{
			Assimp::Importer importer;	//Importer개체 선언 : 3D파일들을 하나의 통일된 데이터 구조로 읽어줌
			// aiScene : 3D 파일을 읽은 후 정점,메쉬,텍스쳐,노드 구조 등 모델 전체 정보를 한곳에 담아두는 최상위 데이터 포인터
			// aiProcess-Trinagulate : 모든 다각형 -> 삼각형
			// FlipUVs : OpenGL 텍스쳐 좌표계에 맟춰 UV 변환(상항 반전 방지)
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
			// 로딩 실패 
			if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				//!scene : 파일 경로가 틀렸거나 손상된 경우
				//AI_SCENE_FLAGS_INCOMPLETE : 파일은 읽었으나 구조가 불완전한 경우
				//!scene->mRootNode : RootNode 데이터가 비어 있는 경우
				cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
				return;
			}
			// 파일의 디렉토리 경로 추출 : 텍스쳐 로딩 시 필요(오브젝트 파일 내부에는 텍스쳐 이미지 경로가 아닌 파일 이름으로만 있는 경우가 많음)
			direcroty = path.substr(0, path.find_last_not_of('/'));

			processNode(scene->mRootNode, scene);
			// 최상위 mRootNode를 넘겨주어 재귀방식으로 모든 메쉬를 하나씩 꺼내옴
		}
		// loadModel에서 넘겨받은 부모-자식관계의 구조를 Root부터 끝까지 탐색, 3D 메쉬 데이터들을 꺼내옴
		void processNode(aiNode* node, const aiScene* scene)
		{

		}

		Mesh porcessMesh(aiMesh* mesh, const aiScene* scene)
		{

		}

		vector<Texture> loadMaterialTextrues(aiMaterial* material, aiTextureType type, string typeName)
		{
			
		}

};