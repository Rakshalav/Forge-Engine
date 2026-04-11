#pragma once

#include "Project/Project.hpp"
#include "AssetManagerBase.hpp"

namespace fg
{
	class AssetManager
	{
		template<typename T>
		static Ref<T> GetAsset(const UUID& handle)
		{
			return std::dynamic_pointer_cast<T>(Project::GetActive()->GetAssetManager()->GetAsset(handle));
		}

		static bool IsAssetHandleValid(const UUID& handle)
		{
			return Project::GetActive()->GetAssetManager()->IsAssetHandleValid(handle);
		}

		static bool IsAssetLoaded(const UUID& handle)
		{
			return Project::GetActive()->GetAssetManager()->IsAssetLoaded(handle);
		}

		static AssetType GetAssetType(const UUID& handle)
		{
			return Project::GetActive()->GetAssetManager()->GetAssetType(handle);
		}
	};
}