#include <glad/glad.h>
#include <stb-image/stb_image.h>
#include "Model.hpp"
#include <assimp/postprocess.h>
#include "../Debug/Log.hpp"
#include "../Maths/Math.hpp"

namespace fg
{
	void Model::Draw(Ref<Shader>& shader)
	{
		for (auto& mesh : m_Meshes)
			mesh.Draw(shader);
	}

	void Model::LoadModel(const std::string& path)
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
		std::vector<Texture> textures;

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

			auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;

			for (unsigned int j = 0; j < m_TextureCache.size(); j++)
			{
				if (std::strcmp(m_TextureCache[j].Path.data(), str.C_Str()) == 0)
				{
					textures.push_back(m_TextureCache[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				Texture texture;
				texture.ID = TextureFromFile(str.C_Str(), this->m_Directory);
				texture.Type = typeName;
				texture.Path = str.C_Str();
				textures.push_back(texture);
				m_TextureCache.push_back(texture);
			}
		}
		return textures;
	}

	uint32_t Model::TextureFromFile(const char* path, const std::string& directory)
	{
		auto filename = std::string(path);
		filename = m_Directory + '/' + filename;

		uint32_t textureID;
		glGenTextures(1, &textureID); 
		glBindTexture(GL_TEXTURE_2D, textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format = 0;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			FG_CORE_ERROR("Texture failed to load at path: {}", path);
			stbi_image_free(data);
		}

		return textureID;
	}
}