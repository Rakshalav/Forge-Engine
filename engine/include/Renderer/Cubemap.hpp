#pragma once

#include <Renderer/Camera.hpp>
#include <Renderer/Texture.hpp>
#include <Asset Manager/Asset.hpp>

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