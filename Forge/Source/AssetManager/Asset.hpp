#pragma once

#include "Core/UUID.hpp"

namespace fg
{
	enum class AssetType
	{
		None = 0,
		Scene,
		Texture2D,
		EnvironmentMap,
		Mesh,
		Material
	};

	class Asset
	{
	public:
		UUID Handle = 0;

		virtual AssetType GetType() const = 0;

	};
}