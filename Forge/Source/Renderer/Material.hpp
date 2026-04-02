#pragma once

#include <string>
#include "Maths/Math.hpp"
#include "Texture.hpp"

namespace fg
{
	enum class BlendMode : uint8_t
	{
		Opaque,
		Masked,
		Translucent
	};

	enum class ShadingModel : uint8_t
	{
		Lit, 
		UnLit
	};

	class Material
	{
	public:
		Material() = default;

		std::string Name;
		std::string ShaderName;

		uint32_t ID;

		struct {
			Vec4f Color				= Vec4f(1.0f);
			Ref<Texture2D> Texture	= nullptr;
		} Albedo;

		struct {
			float Value				= 0.0f;
			Ref<Texture2D> Texture	= nullptr;
		} Metallic;

		struct {
			float Value				= 0.5f;
			Ref<Texture2D> Texture	= nullptr;
		} Roughness;

		Ref<Texture2D> Normal;

		struct {
			float Value				= 0.0f;
			Ref<Texture2D> Texture	= nullptr;
		} AmbientOcclusion;

		struct {
			Vec3f Color				= Vec3f(1.0f);
			Ref<Texture2D> Texture	= nullptr;
			float EmissiveStrength	= 1.0f;
		} Emissive;

		BlendMode BlendMode			= BlendMode::Opaque;
		ShadingModel ShadingModel	= ShadingModel::Lit;
	};
}