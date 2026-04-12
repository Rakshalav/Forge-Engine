#pragma once

#include "Project/Project.hpp"
#include "AssetManagerBase.hpp"

namespace fg::AssetManager
{
	template<typename T>
	static Ref<T> GetAsset(const AssetHandle& handle)
	{
		return std::dynamic_pointer_cast<T>(Project::GetActive()->GetAssetManager()->GetAsset(handle));
	}

	static bool IsAssetHandleValid(const AssetHandle& handle)
	{
		return Project::GetActive()->GetAssetManager()->IsAssetHandleValid(handle);
	}

	static bool IsAssetLoaded(const AssetHandle& handle)
	{
		return Project::GetActive()->GetAssetManager()->IsAssetLoaded(handle);
	}

	static AssetType GetAssetType(const AssetHandle& handle)
	{
		return Project::GetActive()->GetAssetManager()->GetAssetType(handle);
	}
}