#pragma once

#include "../Core/Base.hpp"
#include <string>

namespace fg
{
	class TextureT
	{
	public:
		virtual ~TextureT() = default;
	};

	class Texture2D : public TextureT
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);

		virtual void Bind(uint32_t slot = 0) const = 0;
	};
}

