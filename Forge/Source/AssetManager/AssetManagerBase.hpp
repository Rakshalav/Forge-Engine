#pragma once

#include "Renderer/Texture.hpp"
#include "Renderer/Material.hpp"
#include <unordered_map>
#include <assimp/material.h>
#include <memory>
#include <mutex>
#include "Asset.hpp"
#include <filesystem>

namespace fg
{
    struct AssetMetaData
    {
        AssetType Type;
        std::filesystem::path FilePath;
        std::string Name;
    };

    class AssetManagerBase
    {
    public:
        virtual Ref<Asset> GetAsset(const UUID& handle) = 0;
        virtual bool IsAssetHandleValid(const UUID& handle) const = 0;
        virtual bool IsAssetLoaded(const UUID& handle) const  = 0;
        virtual const AssetMetaData& GetAssetMetaData(const UUID& handle) const = 0;
    };
}