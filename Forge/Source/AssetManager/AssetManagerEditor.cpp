#include "AssetManagerEditor.hpp"
#include "AssetImporter.hpp"
#include "Debug/Log.hpp"
#include "yaml-cpp/yaml.h"
#include "Project/Project.hpp"
#include <fstream>

namespace fg
{
    bool AssetManagerEditor::IsAssetHandleValid(const AssetHandle& handle) const
    {
        return handle != 0 && m_Registry.contains(handle);
    }

    bool AssetManagerEditor::IsAssetLoaded(const AssetHandle& handle) const
    {
        return m_LoadedAssets.contains(handle);
    }

    const AssetMetaData& AssetManagerEditor::GetAssetMetaData(const AssetHandle& handle) const
    {
        return m_Registry.at(handle);
    }

    AssetType AssetManagerEditor::GetAssetType(const AssetHandle& handle) const
    {
        if (!IsAssetHandleValid(handle))
            return AssetType::None;

        return m_Registry.at(handle).Type;
    }

    Ref<Asset> AssetManagerEditor::GetAsset(const AssetHandle& handle)
    {
        std::lock_guard<std::mutex> lock(m_CacheMutex);
        if (!IsAssetHandleValid(handle))
            return nullptr;

        if (IsAssetLoaded(handle))
        {
            auto it = m_LoadedAssets.find(handle);
            if (it != m_LoadedAssets.end())
            {
                if (it->second->RefCount() == 1)
                    m_LoadedAssets.erase(it);
            }
        }

        const AssetMetaData& metadata = GetAssetMetaData(handle);
        Ref<Asset> asset = AssetImporter::ImportAsset(handle, metadata);
        if (!asset)
        {
            FG_ERROR("Failed to import asset!");
            return nullptr;
        }

        m_LoadedAssets[handle] = asset;
        return asset;
    }

    static std::map<AssetType, std::string> sAssetTypeToString = {
        { AssetType::Scene, "Scene" },
        { AssetType::Texture2D, "Texture2D" },
        { AssetType::EnvironmentMap, "EnvironmentMap" },
        { AssetType::Mesh, "Mesh" },
        { AssetType::Material, "Material" },
        { AssetType::None, "None"}
    };

    static std::map<std::string, AssetType> sStringToAssetType = {
        { "Scene", AssetType::Scene },
        { "Texture2D", AssetType::Texture2D },
        { "EnvironmentMap", AssetType::EnvironmentMap },
        { "Mesh", AssetType::Mesh },
        { "Material", AssetType::Material },
        { "None", AssetType::None }
    };

    bool AssetManagerEditor::SerializeAssetRegistry()
    {
        YAML::Emitter out;
        {
            out << YAML::BeginMap;
            out << YAML::Key << "Assets" << YAML::Value;
            out << YAML::BeginMap; 

            for (const auto& [handle, metadata] : m_Registry)
            {
                std::stringstream ss;
                ss << std::hex << (uint64_t)handle;
                std::string handleStr = ss.str();

                out << YAML::Key << handleStr;
                out << YAML::BeginMap;
                {
                    out << YAML::Key << "Type" << YAML::Value << sAssetTypeToString[metadata.Type];
                    out << YAML::Key << "Path" << YAML::Value << metadata.FilePath.string();
                }
                out << YAML::EndMap;
            }
            out << YAML::EndMap; 
            out << YAML::EndMap; 
        }

        std::ofstream fout(Project::GetAssetRegistryPath());
        if (!fout.is_open())
            return false;
        fout << out.c_str();
        return true;
    }

    bool AssetManagerEditor::DeserializeAssetRegistry()
    {
        YAML::Node data;
        try
        {
            data = YAML::LoadFile(Project::GetAssetRegistryPath().string());
        }
        catch (const YAML::ParserException& e)
        {
            FG_ERROR("Failed to read Asset Registry '{}'\n {}", Project::GetAssetRegistryPath().string(), e.what());
            return false;
        }

        auto assets = data["Assets"];
        if (!assets || !assets.IsMap())
            return false;

        for (auto it = assets.begin(); it != assets.end(); it++)
        {
            std::string handleStr = it->first.as<std::string>();
            uint64_t rawHandleValue = std::stoull(handleStr, nullptr, 16);
            AssetHandle handle(rawHandleValue);

            auto entry = it->second;

            AssetMetaData metadata;
            metadata.Type = sStringToAssetType[entry["Type"].as<std::string>()];
            metadata.FilePath = entry["Path"].as<std::string>();

            m_Registry.emplace(handle, metadata);
            m_PathToHandle.emplace(metadata.FilePath, handle);
        }

        return true;
    }

    AssetHandle AssetManagerEditor::GetHandleFromRelativePath(const std::filesystem::path& path)
    {
        auto it = m_PathToHandle.find(path);
        return it != m_PathToHandle.end() ? it->second : 0;
    }

    std::string AssetManagerEditor::AssetTypeToString(AssetType type)
    {
        return sAssetTypeToString[type];
    }

    static const std::unordered_map<std::string, AssetType> s_ExtensionToAssetType = {
        { ".png",  AssetType::Texture2D },
        { ".jpg",  AssetType::Texture2D },
        { ".jpeg", AssetType::Texture2D },

        { ".fbx",  AssetType::Mesh },
        { ".obj",  AssetType::Mesh },
        { ".gltf", AssetType::Mesh },

        { ".mat",  AssetType::Material },

        { ".scene", AssetType::Scene },

        { ".hdr", AssetType::EnvironmentMap }
    };

    void AssetManagerEditor::RegisterAsset(const std::filesystem::path& path)
    {
        AssetHandle handle;
        AssetMetaData metaData;

        metaData.FilePath = path;
        auto it = s_ExtensionToAssetType.find(path.extension().string());
        it != s_ExtensionToAssetType.end() ? metaData.Type = it->second : metaData.Type = AssetType::None;

        m_Registry.emplace(handle, metaData);
        m_PathToHandle.emplace(metaData.FilePath, handle);
    }

    void AssetManagerEditor::UnRegisterAsset(const AssetHandle& handle)
    {
        if (IsAssetHandleValid(handle))
        {
            auto it = m_Registry.find(handle);
            m_PathToHandle.erase(it->second.FilePath);
            m_Registry.erase(handle);
        }
        else
        {
            std::stringstream ss;
            ss << std::hex << (uint64_t)handle;
            FG_ERROR("Invalid AssetHandle: {}", ss.str());
        }
    }
}