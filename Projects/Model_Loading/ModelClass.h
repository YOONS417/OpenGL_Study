#ifndef MODEL_H
#define MODEL_H
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
#include "Texture.h"
using namespace std;
// 텍스쳐 파일 로드용 함수

//unsigned  int TextureFromFile(const char* path, const std::string& directory);

class Model
{
	public:
		vector<Mesh> meshes;
		string directory;

		vector<Texture> loaded_texture;  
		// 중복 로딩을 방지하기 위한 모든 텍스쳐의 정보(ID,경로)를 보관하는 중앙 cache 역할

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
			directory = path.substr(0, path.find_last_not_of('/'));

			processNode(scene->mRootNode, scene);
			// 최상위 mRootNode를 넘겨주어 재귀방식으로 모든 메쉬를 하나씩 꺼내옴
		}
		// loadModel에서 넘겨받은 부모-자식관계의 구조를 Root부터 끝까지 탐색, 3D 메쉬 데이터들을 꺼내옴
		void processNode(aiNode* node, const aiScene* scene)
		{
			// 현재 노드에 속한 모든 메쉬를 메쉬 개수만큼 순회 및 처리
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				// node->mMeshes[i] : 실제 메쉬의 인덱스 번호
				// 메쉬 데이터는 scene->mMeshes 중앙 배열에 있음
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				// 두 번 건너뛰어 접근 : 노드 자체에는 정점,텍스쳐 좌표가 없음
				// 찾아낸 aiMesh포이터를 processMesh로 전달
				// 메쉬 데이터를 변환하여 Model의 meshes 배열에 추가
				meshes.push_back(processMesh(mesh, scene));
			}
			// 현재 노드의 자식 노드들에 대한 재귀(자식 노드 개수만큼)
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);	//mChildren : 자식 노드 배열
			}
		}
		// Assimp가 읽어온 데이터 구조체(aiMesh) -> OpenGL 규격의 Mesh객체(vertices,indices,textrue)로 변환
		Mesh processMesh(aiMesh* mesh, const aiScene* scene)
		{	
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;

			for (unsigned int i=0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;

				// 위치 
				vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				// 법선
				if (mesh->HasNormals())
				{
					vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				}
				// 텍스쳐 좌표
				if (mesh->mTextureCoords[0])
				{
					vertex.TexCoord = glm::vec2(mesh->mTextureCoords[i]->x, mesh->mTextureCoords[i]->y);
				}
				else
					vertex.TexCoord = glm::vec2(0.0f, 0.0f);

				vertices.push_back(vertex);
			}
			// 인덱스 데이터 추출(EBO에 들어갈 정보)
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				// aiFace : 하나의 면(폴리곤)
				// loadModel에서 Triangulat 옵션 -> 모든 face는 삼각형
				aiFace face = mesh->mFaces[i];  //메쉬를 구성하는 모든 '면'객체들의 배열

				for (unsigned int j = 0; j < face.mNumIndices; j++) // 면을 이루는 인덱스 개수 만큼
					indices.push_back(face.mIndices[j]);	// 정점 번호 배열
			}
			// aiMesh 안에는 자신이 사용하는 인덱스 번호만 있음 -> 이 번호로 scene->mMaterials배열로 가서 aiMaterial객체를 찾아옴
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// loadMaterialTextures 함수로 이미지(diffuse,specular)를 불ㄹ와 textures배열에 합침(insert)
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// C++ Mesh 클래스 생성자로 넘겨 Mesh객체로 변환
			// 생성자가 실행되면서 내부의 setupMesh()가 작동해 VBO,EBO가 생성
			return Mesh(vertices, indices, textures);
		}

		vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName)
		{
			vector<Texture> textures; // 개별 Mesh 전용 리스트

			// 해당 타입의 텍스쳐가 Material에 몇 개 있는지 확인
			for (unsigned int i = 0; i < material->GetTextureCount(type); i++) 
			{
				// material에 저장된 텍스쳐의 갯수 확인
				aiString str;
				// 텍스쳐 이미지 파일의 상대경로("~.png")를 str에 저장
				material->GetTexture(type, i, &str);

				// 이미 도르된 텍스쳐인지 검사 : Caching
				// 모델은 Mesh파츠로 나뉘어 있지만, 동일한 텍스쳐 파일을 공유하는 경우가 많음
				// 중복 검사를 거쳐 GPU메모리에 중복 생성, 메모리 낭비, 프레임 드랍 방지
				bool skip = false;
				for (unsigned int j = 0; j < loaded_texture.size(); j++)
				{
					// 경로 문자열 비교(동일한 파일인지 검사)
					if (strcmp(loaded_texture[j].path.data(), str.C_Str()) == 0) // 동일한 파일이 있는 경우
					{
						textures.push_back(loaded_texture[j]);  // 이미 존재한 파일을 GPU에 올리지 않고, ID만 텍스쳐 목록(textures)에 전달
						skip = true;	// 중복은 스킵
						break;
					}
				}
				// cache에 없는 새로운 텍스쳐만 새로 로드
				if (!skip)
				{
					Texture texture;
					// 이미지를 읽어 GPU 메모리에 올리고 텍스쳐 ID 반환
					texture.id = LoadTexture(string(str.C_Str()), true);
					texture.type = typeName;
					texture.path = str.C_Str();

					textures.push_back(texture);  // 현재 메쉬에게 이 ID를 사용하라고 전달
					loaded_texture.push_back(texture);	// 새로운 텍스쳐를 cache 리스트에 저장
				}
			}
			return textures;
		}
};
#endif