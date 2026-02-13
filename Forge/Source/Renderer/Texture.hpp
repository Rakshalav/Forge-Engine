#pragma once

#include "../Core/Base.hpp"
#include <string>

namespace fg
{
	class Texture
	{
	public:
		virtual ~Texture() = default;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);

		virtual void Bind(uint32_t slot = 0) const = 0;
	};
}

