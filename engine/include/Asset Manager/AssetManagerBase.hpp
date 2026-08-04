#pragma once

#include <Renderer/Texture.hpp>
#include <Renderer/Material.hpp>
#include <fgpch.hpp>
#include <mutex>
#include "Asset.hpp"
#include <filesystem>
#include <variant>

namespace fg
{
    struct AssetMetaData
    {
        AssetType Type;
        std::filesystem::path FilePath;
    };

    class AssetManagerBase : public RefCounted
    {
    public:
        virtual Ref<Asset> GetAsset(const AssetHandle& handle) = 0;
        virtual bool IsAssetHandleValid(const AssetHandle& handle) const = 0;
        virtual bool IsAssetLoaded(const AssetHandle& handle) const  = 0;
        virtual const AssetMetaData& GetAssetMetaData(const AssetHandle& handle) const = 0;
        virtual AssetType GetAssetType(const AssetHandle& handle) const = 0;
    };
}