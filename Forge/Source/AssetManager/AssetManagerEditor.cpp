#include "AssetManagerEditor.hpp"

namespace fg
{
    bool AssetManagerEditor::IsAssetHandleValid(const UUID& handle) const
    {
        return handle != 0 && m_Registry.contains(handle);
    }

    bool AssetManagerEditor::IsAssetLoaded(const UUID& handle) const
    {
        return m_LoadedAssets.contains(handle);
    }

    const AssetMetaData& AssetManagerEditor::GetAssetMetaData(const UUID& handle) const
    {
        return m_Registry.at(handle);
    }

    Ref<Asset> AssetManagerEditor::GetAsset(const UUID& handle)
    {
        std::lock_guard<std::mutex> lock(m_CacheMutex);
        //1. Check if Handle is valid.
        if (!IsAssetHandleValid(handle))
            return nullptr;

        //2. Check if asset is loaded
        if (IsAssetLoaded(handle)) {
            auto it = m_LoadedAssets.find(handle);
            if (Ref<Asset> ref = it->second.lock())
                return ref;
            else
                m_LoadedAssets.erase(it);
        }

        //3. if asset not loaded in memory then load it from registry
        const AssetMetaData& metadata = GetAssetMetaData(handle);
        //Ref<Asset> asset = AssetImporter::ImportAsset(metadata);

        //if (!asset)
        //    return asset;
    }
}

