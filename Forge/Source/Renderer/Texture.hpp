#pragma once

#include "../Core/Base.hpp"
#include <Maths/Math.hpp>
#include <string>

namespace fg
{
	enum class TextureType : uint8_t
	{
		NONE = 0,
		DIFFUSE,
		SPECULAR,
		AMBIENT,
		EMMISIVE,
		NORMAL
	};

	enum class TextureFormat : uint8_t
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		TextureFormat InternalFormat = TextureFormat::None;
		uint32_t DataFormat = 0;
		TextureType Type = TextureType::NONE;
	};

	class Texture2D
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const TextureSpecification& specification);

		virtual void Bind() const = 0;
		virtual void Activate(uint32_t slot = 0) const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual Vec2u GetSize() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetType(TextureType type) = 0;
		virtual TextureType GetType() const = 0;
		virtual std::string GetPath() const = 0;

		virtual bool operator== (const Texture2D& other) const = 0;
	};
}