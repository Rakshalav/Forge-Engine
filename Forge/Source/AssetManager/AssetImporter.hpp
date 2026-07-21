#pragma once

#include "Core/Memory.hpp"
#include "Asset.hpp"
#include "AssetManagerBase.hpp"

namespace fg
{
	struct AssetImporter
	{
		static Ref<Asset> ImportAsset(const AssetHandle& handle, const AssetMetaData& metadata);
	};
}