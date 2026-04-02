//#include "FileSystem.hpp"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include <functional>
//#include <mutex>
//#include "Core/TaskManager.hpp"
//#include "Debug/Log.hpp"
//
//namespace fg
//{
//    Ref<Mesh> FileSystem::LoadModel(fs::path path)
//    {
//        Ref<Mesh> modelMesh = CreateRef<Mesh>();
//        auto loaderMutex = std::make_shared<std::mutex>();
//
//        TaskManager::Get().Submit(new FunctionTask([path, modelMesh, loaderMutex]() 
//        {
//            auto importer = std::make_shared<Assimp::Importer>();
//            const aiScene* scene = importer->ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);
//
//            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
//            {
//                FG_ERROR("ASSIMP Error: {}", importer->GetErrorString());
//                return;
//            }
//
//            std::string dir = path.parent_path().string();
//
//            std::function<void(aiNode*)> processNode = [&](aiNode* node) 
//            {
//                for (unsigned int i = 0; i < node->mNumMeshes; i++) 
//                {
//                    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//
//                    TaskManager::Get().Submit(new FunctionTask([mesh, scene, dir, modelMesh, importer, loaderMutex]() 
//                    {
//                        std::vector<Vertex> vertices;
//                        std::vector<uint32_t> indices;
//
//                        vertices.reserve(mesh->mNumVertices);
//                        for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
//                        {
//                            Vertex v;
//                            v.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
//                            v.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
//
//                            if (mesh->mTextureCoords[0]) 
//                            {
//                                v.TextureCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
//                                v.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
//                                v.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
//                            }
//                            vertices.push_back(v);
//                        }
//
//                        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//                            for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
//                                indices.push_back(mesh->mFaces[i].mIndices[j]);
//
//                        if (mesh->mMaterialIndex >= 0)
//                        {
//                            auto* aiMat = scene->mMaterials[mesh->mMaterialIndex];
//
//                            std::string matName = aiMat->GetName().C_Str();
//                            //Ref<Material> mat = AssetManager::Get().GetMaterial(matName, aiMat, dir);
//                        }
//
//                        {
//                            std::lock_guard<std::mutex> lock(*loaderMutex);
//                            modelMesh->SubMeshes.emplace_back(vertices, indices);
//                        }
//                    }));
//                }
//
//                for (unsigned int i = 0; i < node->mNumChildren; i++)
//                    processNode(node->mChildren[i]);
//            };
//
//            processNode(scene->mRootNode);
//        }));
//
//        return modelMesh;
//    }
//}