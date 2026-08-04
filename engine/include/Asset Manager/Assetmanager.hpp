#pragma once

#include <Project/Project.hpp>
#include "AssetManagerBase.hpp"

namespace fg
{
	class AssetManager
	{
	public:
		template<typename T>
		static Ref<T> GetAsset(const AssetHandle& handle) {
			return DynamicRefCast<T>(Project::GetActive()->GetAssetManager()->GetAsset(handle));
		}

		template<typename T>
		Ref<T> operator[](const AssetHandle& handle) {
			return DynamicRefCast<T>(Project::GetActive()->GetAssetManager()->GetAsset(handle));
		}

		static bool IsAssetHandleValid(const AssetHandle& handle) {
			return Project::GetActive()->GetAssetManager()->IsAssetHandleValid(handle);
		}

		static bool IsAssetLoaded(const AssetHandle& handle) {
			return Project::GetActive()->GetAssetManager()->IsAssetLoaded(handle);
		}

		static AssetType GetAssetType(const AssetHandle& handle) {
			return Project::GetActive()->GetAssetManager()->GetAssetType(handle);
		}

		static const AssetMetaData& GetAssetMetaData(const AssetHandle& handle) 
		{
			return Project::GetActive()->GetAssetManager()->GetAssetMetaData(handle);
		}
	};
}