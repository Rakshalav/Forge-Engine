#pragma once

#include "Core/Base.hpp"
#include "Asset.hpp"
#include "AssetManagerBase.hpp"

namespace fg
{
	class AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(const UUID& handle, const AssetMetaData& metadata);
	};
}