#pragma once

#include "Core/Base.hpp"
#include "Renderer/Camera.hpp"
#include "Texture.hpp"
#include "AssetManager/Asset.hpp"

namespace fg
{
	class EnvironmentMap : public Asset
	{
	public:
		virtual void Draw(Camera& camera) = 0;
		virtual AssetType GetAssetType() = 0;
		static AssetType GetStaticAssetType() { return AssetType::EnvironmentMap; }
		static Ref<EnvironmentMap> Create(Ref<Texture2D> texture);
	};
}