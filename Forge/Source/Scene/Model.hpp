#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Mesh.hpp"

namespace fg
{
	class Model
	{
	public:
		Model(std::string path) { LoadModel(path); }
		
		void Draw(Ref<Shader>& shader);

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		uint32_t TextureFromFile(const char* path, const std::string& directory);

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Texture> m_TextureCache;
		std::string m_Directory;
	};
}