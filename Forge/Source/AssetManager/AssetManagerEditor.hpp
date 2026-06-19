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

		AssetHandle GetHandleFromRelativePath(const std::filesystem::path& path);

		std::string AssetTypeToString(AssetType type);

		void RegisterAsset(const std::filesystem::path& path);
		void UnRegisterAsset(const AssetHandle& handle);

	private:
		AssetRegistry<AssetMetaData> m_Registry;
		AssetRegistry<Ref<Asset>> m_LoadedAssets;

		std::unordered_map<std::filesystem::path, AssetHandle> m_PathToHandle;

		std::mutex m_CacheMutex;
	};
}