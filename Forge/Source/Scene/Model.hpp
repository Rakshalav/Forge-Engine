#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Mesh.hpp"

namespace fg
{
	class Model
	{
	public:
		Model(std::string path);
		
		void Draw(Ref<Shader>& shader);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial * mat, aiTextureType type, TextureType FG_type);

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Ref<Texture2D>> m_TextureCache;
		std::string m_Directory;
	};
}