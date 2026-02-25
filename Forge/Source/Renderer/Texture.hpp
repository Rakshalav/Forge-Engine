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

	class Texture2D
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual Vec2u GetSize() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetType(TextureType type) = 0;
		virtual TextureType GetType() const = 0;
		virtual std::string GetPath() const = 0;

		virtual bool operator== (const Texture2D& other) const = 0;
	};
}

