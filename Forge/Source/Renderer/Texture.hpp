#pragma once

#include "../Core/Base.hpp"
#include <string>

namespace Forge
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
	};
}

