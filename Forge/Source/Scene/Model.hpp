#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include "Mesh.hpp"

namespace fg
{
    class Model
    {
    public:
        Model(std::string path);
        void Draw(Ref<Shader>& shader);

    private:
        struct MeshData
        {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            std::vector<std::pair<std::string, TextureType>> textureSpecs;
        };

    private:
        void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<Assimp::Importer> keepAlive);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Assimp::Importer> keepAlive);
        void FillTextureSpecs(std::vector<std::pair<std::string, TextureType>>& specs, aiMaterial* mat, aiTextureType type, TextureType FG_type);

    private:
        std::vector<Mesh> m_Meshes;
        std::mutex m_MeshMutex;
        std::string m_Directory;
    };
}