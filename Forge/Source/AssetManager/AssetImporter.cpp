#include "Core/TaskManager.hpp"
#include "Project/Project.hpp"
#include "AssetImporter.hpp"
#include "Debug/Log.hpp"
#include <functional>
#include <map>

#include <stb-image/stb_image.h>
#include "Renderer/Texture.hpp"
#include "Renderer/Cubemap.hpp"

#include "Scene/Mesh.hpp"
#include <assimp/material.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace fg
{
#pragma region Texture
    static Ref<Texture2D> ImportTexture2D(const AssetHandle& handle, const AssetMetaData& metadata)
    {
        std::filesystem::path path = Project::GetActive()->GetAssetDirectory() / metadata.FilePath;

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(1);

        unsigned char* pixels = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
        if (!pixels)
        {
            FG_CORE_ERROR("Failed to load texture at '{}'", path.string());
            return nullptr;
        }

        TextureSpecification spec;
        spec.Width = width;
        spec.Height = height;

        auto& texConfig = metadata.GetConfig<Texture2DConfig>();
        spec.WrapMode = texConfig.WrapMode;
        spec.FilterMode = texConfig.FilterMode;
        spec.GenerateMipMap = texConfig.GenerateMipMap;

        switch (nrChannels)
        {
        case 1: 
            spec.Format = TextureSpecification::InternalFormat::R8; 
            break;
        case 3: 
            spec.Format = TextureSpecification::InternalFormat::RGB8; 
            break;
        case 4: 
            spec.Format = TextureSpecification::InternalFormat::RGBA8; 
            break;
        }

        auto texture = Texture2D::Create(spec);
        texture->Handle = handle;
        texture->SetData(pixels, width * height * nrChannels);

        stbi_image_free(pixels);
        return texture;
    }

    static Ref<EnvironmentMap> ImportEnvironmentMap(const AssetHandle& handle, const AssetMetaData& metadata)
    {
        std::filesystem::path path = Project::GetActive()->GetAssetDirectory() / metadata.FilePath;

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(1);

        float* pixels = stbi_loadf(path.string().c_str(), &width, &height, &nrChannels, 0);
        if (!pixels)
        {
            FG_CORE_ERROR("Failed to load environment map at '{}'", path.string());
            return nullptr;
        }

        TextureSpecification spec;
        spec.Width = width;
        spec.Height = height;
        spec.Format = TextureSpecification::InternalFormat::RGB32F;
        spec.WrapMode = TextureSpecification::Wrap::ClampToEdge;
        spec.GenerateMipMap = false;

        auto equirectTexture = Texture2D::Create(spec);
        equirectTexture->SetData(pixels, width * height * nrChannels * (int)sizeof(float));

        stbi_image_free(pixels);
        auto envMap = EnvironmentMap::Create(equirectTexture);
        envMap->Handle = handle;
    }
#pragma endregion and HDR files importer

#pragma region Mesh
    static Ref<Mesh> ImportMesh(const AssetHandle& handle, const AssetMetaData& metadata)
    {
        std::filesystem::path path = Project::GetActive()->GetAssetDirectory() / metadata.FilePath;

        Ref<Mesh> modelMesh = CreateRef<Mesh>();
        auto loaderMutex = std::make_shared<std::mutex>();

        TaskManager::Get().Submit(new FunctionTask([path, modelMesh, loaderMutex]()
        {
            auto importer = std::make_shared<Assimp::Importer>();
            const aiScene* scene = importer->ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                FG_ERROR("ASSIMP Error: {}", importer->GetErrorString());
                return;
            }

            std::string dir = path.parent_path().string();
            std::function<void(aiNode*)> processNode = [&](aiNode* node)
            {
                for (unsigned int i = 0; i < node->mNumMeshes; i++)
                {
                    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                    TaskManager::Get().Submit(new FunctionTask([mesh, scene, dir, modelMesh, importer, loaderMutex]()
                    {
                        std::vector<Vertex> vertices;
                        std::vector<uint32_t> indices;

                        vertices.reserve(mesh->mNumVertices);
                        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
                        {
                            Vertex v;
                            v.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
                            v.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

                            if (mesh->mTextureCoords[0])
                            {
                                v.TextureCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                                v.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
                                v.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
                            }
                            vertices.push_back(v);
                        }

                        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
                            for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
                                indices.push_back(mesh->mFaces[i].mIndices[j]);

                        if (mesh->mMaterialIndex >= 0)
                        {
                            auto* aiMat = scene->mMaterials[mesh->mMaterialIndex];
                            std::string matName = aiMat->GetName().C_Str();
                            //Ref<Material> mat = AssetManager::Get().GetMaterial(matName, aiMat, dir);
                        }

                        {
                            std::lock_guard<std::mutex> lock(*loaderMutex);
                            modelMesh->SubMeshes.emplace_back(vertices, indices);
                        }
                    }));
                }

                for (unsigned int i = 0; i < node->mNumChildren; i++)
                    processNode(node->mChildren[i]);
            };
            processNode(scene->mRootNode);
        }));
        return modelMesh;
    }
#pragma endregion Importer

	using AssetImportFunction = std::function<Ref<Asset>(const AssetHandle&, const AssetMetaData&)>;

    static std::map <AssetType, AssetImportFunction> s_AssetImportFunctions = {
        { AssetType::Texture2D, ImportTexture2D },
        { AssetType::EnvironmentMap, ImportEnvironmentMap },
        { AssetType::Mesh, ImportMesh }
	};

	Ref<Asset> AssetImporter::ImportAsset(const AssetHandle& handle, const AssetMetaData& metadata)
	{
		return s_AssetImportFunctions.at(metadata.Type)(handle, metadata);
	}
}