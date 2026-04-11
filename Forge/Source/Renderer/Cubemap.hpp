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
		AssetType GetType() const override { return AssetType::EnvironmentMap; }
		static Ref<EnvironmentMap> Create(Ref<Texture2D> texture);
	};
}