#include "Model.hpp"
#include <assimp/postprocess.h>
#include "../Debug/Log.hpp"
#include "../Maths/Math.hpp"

namespace fg
{
	Model::Model(std::string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			FG_CORE_ERROR("ASSIMP: {}", import.GetErrorString());
			return;
		}
		m_Directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::Draw(Ref<Shader>& shader)
	{
		for (auto& mesh : m_Meshes)
			mesh.Draw(shader);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], scene);
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector <Ref<Texture2D>> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			Vec3f vector;
			auto& MeshVertices = mesh->mVertices;
			vector.x = MeshVertices[i].x;
			vector.y = MeshVertices[i].y;
			vector.z = MeshVertices[i].z;
			vertex.Position = vector;

			Vec3f normal;
			auto& MeshNormals = mesh->mNormals;
			normal.x = MeshNormals[i].x;
			normal.y = MeshNormals[i].y;
			normal.z = MeshNormals[i].z;
			vertex.Normal = normal;

			if (mesh->mTextureCoords[0])
			{
				Vec2f texCoords;
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;
				vertex.TextureCoords = texCoords;

				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;

				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			else
			{
				vertex.TextureCoords = { 0.0f, 0.0f };
				vertex.Tangent = { 0.0f, 0.0f, 0.0f };
				vertex.Bitangent = { 0.0f, 0.0f, 0.0f };
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0)
		{
			auto* material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			auto normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::NORMAL);
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			auto ambientMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::AMBIENT);
			textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType FG_type)
	{
		std::vector <Ref<Texture2D>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;

			for (unsigned int j = 0; j < m_TextureCache.size(); j++)
			{
				if (std::strcmp(m_TextureCache[j]->GetPath().data(), str.C_Str()) == 0)
				{
					textures.push_back(m_TextureCache[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				std::string path = str.C_Str();
				path = m_Directory + '/' + path;
				Ref<Texture2D> texture = Texture2D::Create(path);
				texture->SetType(FG_type);
				textures.push_back(texture);
				m_TextureCache.push_back(texture);
			}
		}
		return textures;
	}
}