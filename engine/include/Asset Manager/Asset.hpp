#pragma once

#include <Core/UUID.hpp>
#include <fgpch.hpp>

namespace fg
{
	using AssetHandle = UUID;

	enum class AssetType : uint8_t
	{
		None = 0,
		Scene,
		Texture2D,
		EnvironmentMap,
		Mesh,
		Material
	};

	class Asset : public RefCounted
	{
	public:
		AssetHandle Handle = 0;
		virtual ~Asset() = default;
		virtual AssetType GetType() const = 0;
	};
}