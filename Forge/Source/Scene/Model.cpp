#include "Model.hpp"
#include <assimp/postprocess.h>
#include "Debug/Log.hpp"
#include "Core/TaskManager.hpp"
#include "Renderer/RenderCommand.hpp"

namespace fg
{
    Model::Model(std::string path)
    {
        TaskManager::Get().Submit(new FunctionTask([this, path]() {
            auto import = std::make_shared<Assimp::Importer>();
            const aiScene* scene = import->ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                FG_ERROR("ASSIMP: {}", import->GetErrorString());
                return;
            }

            m_Directory = path.substr(0, path.find_last_of('/'));

            ProcessNode(scene->mRootNode, scene, import);
            }));
    }

    void Model::Draw(Ref<Shader>& shader)
    {
        std::lock_guard<std::mutex> lock(m_MeshMutex);
        for (auto& mesh : m_Meshes)
            mesh.Draw(shader);
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<Assimp::Importer> keepAlive)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene, keepAlive);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
            ProcessNode(node->mChildren[i], scene, keepAlive);
    }

    void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Assimp::Importer> keepAlive)
    {
        TaskManager::Get().Submit(new FunctionTask([this, mesh, scene, keepAlive]() {
            MeshData data;

            data.vertices.reserve(mesh->mNumVertices);
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
                vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

                if (mesh->mTextureCoords[0])
                {
                    vertex.TextureCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                    vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
                    vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
                }
                data.vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    data.indices.push_back(face.mIndices[j]);
            }

            if (mesh->mMaterialIndex >= 0)
            {
                auto* material = scene->mMaterials[mesh->mMaterialIndex];
                FillTextureSpecs(data.textureSpecs, material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
                FillTextureSpecs(data.textureSpecs, material, aiTextureType_SPECULAR, TextureType::SPECULAR);
                FillTextureSpecs(data.textureSpecs, material, aiTextureType_HEIGHT, TextureType::NORMAL);
            }

            RenderCommand::Submit([this, meshData = std::move(data)]() mutable {
                std::vector<Ref<Texture2D>> textures;
                for (auto& spec : meshData.textureSpecs)
                {
                    auto tex = Texture2D::Create(spec.first);
                    tex->SetType(spec.second);
                    textures.push_back(tex);
                }

                Mesh finalMesh(meshData.vertices, meshData.indices, textures);

                std::lock_guard<std::mutex> lock(m_MeshMutex);
                m_Meshes.push_back(std::move(finalMesh));

                FG_TRACE("Mesh finalized and uploaded to GPU.");
                });
            }));
    }

    void Model::FillTextureSpecs(std::vector<std::pair<std::string, TextureType>>& specs, aiMaterial* mat, aiTextureType type, TextureType FG_type)
    {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string fullPath = m_Directory + '/' + str.C_Str();
            specs.push_back({ fullPath, FG_type });
        }
    }