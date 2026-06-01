#pragma once

#include "AssetManagerBase.hpp"
#include <unordered_map>

namespace fg
{
	template<typename T>
	using AssetRegistry = std::unordered_map<AssetHandle, T>;

	class AssetManagerEditor : public AssetManagerBase
	{
	public:
		Ref<Asset> GetAsset(const AssetHandle& handle) override;
		bool IsAssetHandleValid(const AssetHandle& handle) const override;
		bool IsAssetLoaded(const AssetHandle& handle) const override;
		const AssetMetaData& GetAssetMetaData(const AssetHandle& handle) const override;
		AssetType GetAssetType(const AssetHandle& handle) const override;

		bool SerializeAssetRegistry();
		bool DeserializeAssetRegistry();

	private:
		AssetRegistry<AssetMetaData> m_Registry;
		AssetRegistry<Ref<Asset>> m_LoadedAssets;

		std::mutex m_CacheMutex;
	};
}