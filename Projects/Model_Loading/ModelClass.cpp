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

		void Draw(Shader shader)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(shader);
		}

	private:
		void loadModel(string const& path)
		{
			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		}

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