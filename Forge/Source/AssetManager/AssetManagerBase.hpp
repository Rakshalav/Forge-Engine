#pragma once

#include "Renderer/Texture.hpp"
#include "Renderer/Material.hpp"
#include <unordered_map>
#include <memory>
#include <mutex>
#include "Asset.hpp"
#include <filesystem>
#include <variant>

namespace fg
{
    struct Texture2DConfig
    {
        TextureSpecification::Wrap WrapMode = TextureSpecification::Wrap::Repeat;
        TextureSpecification::Filter FilterMode = TextureSpecification::Filter::Linear;
        bool GenerateMipMap = true;
    };

    struct AssetMetaData
    {
        AssetType Type;
        std::filesystem::path FilePath;
        std::variant<std::monostate, Texture2DConfig> Config;

        template<typename T>
        bool HasConfig() const { return std::holds_alternative<T>(Config); }

        template<typename T>
        const T& GetConfig() const { 
            if (!HasConfig<T>())
                assert("Metadata does not contain the requested config type!");
           return std::get<T>(Config); 
        }
    };

    class AssetManagerBase
    {
    public:
        virtual Ref<Asset> GetAsset(const AssetHandle& handle) = 0;
        virtual bool IsAssetHandleValid(const AssetHandle& handle) const = 0;
        virtual bool IsAssetLoaded(const AssetHandle& handle) const  = 0;
        virtual const AssetMetaData& GetAssetMetaData(const AssetHandle& handle) const = 0;
        virtual AssetType GetAssetType(const AssetHandle& handle) const = 0;
    };
}