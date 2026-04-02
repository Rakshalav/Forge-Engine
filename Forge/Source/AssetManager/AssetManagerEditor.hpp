#pragma once

#include "AssetManagerBase.hpp"
#include <unordered_map>

namespace fg
{
	template<typename T>
	using AssetRegistry = std::unordered_map<UUID, T>;

	class AssetManagerEditor : public AssetManagerBase
	{
	public:
		Ref<Asset> GetAsset(const UUID& handle) override;
		bool IsAssetHandleValid(const UUID& handle) const override;
		bool IsAssetLoaded(const UUID& handle) const override;
		const AssetMetaData& GetAssetMetaData(const UUID& handle) const override;

	private:
		AssetRegistry<AssetMetaData> m_Registry;
		AssetRegistry<std::weak_ptr<Asset>> m_LoadedAssets;

		std::mutex m_CacheMutex;
	};
}