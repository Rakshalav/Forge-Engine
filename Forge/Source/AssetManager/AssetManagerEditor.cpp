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

    static std::map<AssetType, std::string> AssetTypeToString = {
        { AssetType::Scene, "Scene" },
        { AssetType::Texture2D, "Texture2D" },
        { AssetType::EnvironmentMap, "EnvironmentMap" },
        { AssetType::Mesh, "Mesh" },
        { AssetType::Material, "Material" }
    };

    static std::map<std::string, AssetType> StringToAssetType = {
        { "Scene", AssetType::Scene },
        { "Texture2D", AssetType::Texture2D },
        { "EnvironmentMap", AssetType::EnvironmentMap },
        { "Mesh", AssetType::Mesh },
        { "Material", AssetType::Material }
    };

    bool AssetManagerEditor::SerializeAssetRegistry()
    {
        YAML::Emitter out;
        {
            out << YAML::BeginMap;
            out << YAML::Key << "Assets" << YAML::Value;
            for (const auto& [handle, metadata] : m_Registry)
            {
                out << YAML::Key << YAML::Hex << (uint64_t)handle;
                out << YAML::BeginMap;
                {
                    out << YAML::Key << "Type" << YAML::Value << AssetTypeToString[metadata.Type];
                    out << YAML::Key << "Path" << YAML::Value << metadata.FilePath.string();

                    if (metadata.HasConfig<Texture2DConfig>())
                    {
                        auto& config = metadata.GetConfig<Texture2DConfig>();
                        out << YAML::Key << "Wrap" << YAML::Value << (int)config.WrapMode;
                        out << YAML::Key << "Filter" << YAML::Value << (int)config.FilterMode;
                        out << YAML::Key << "MipMap" << YAML::Value << config.GenerateMipMap;
                    }
                }
                out << YAML::EndMap;
            }
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
        catch (YAML::ParserException e)
        {
            FG_ERROR("Failed to load Asset Registry '{}'\n {}", Project::GetAssetRegistryPath().string(), e.what());
            return false;
        }

        auto assets = data["Assets"];
        if (!assets || !assets.IsMap()) 
            return false;

        for (auto it = assets.begin(); it != assets.end(); it++)
        {
            AssetHandle handle(it->first.as<uint64_t>());
            auto entry = it->second;

            AssetMetaData metadata;
            metadata.Type = StringToAssetType[entry["Type"].as<std::string>()];
            metadata.FilePath = entry["Path"].as<std::string>();

            switch (metadata.Type)
            {
            case AssetType::Texture2D:
            {
                Texture2DConfig texConfig;
                if (entry["Wrap"]) texConfig.WrapMode = (TextureSpecification::Wrap)entry["Wrap"].as<int>();
                if (entry["Filter"]) texConfig.FilterMode = (TextureSpecification::Filter)entry["Filter"].as<int>();
                if (entry["MipMap"]) texConfig.GenerateMipMap = entry["MipMap"].as<bool>();

                metadata.Config = texConfig;
                break;
            }
            default:
                metadata.Config = std::monostate{};
                break;
            }

            m_Registry.emplace(handle, metadata);
        }
        return true;
    }
}