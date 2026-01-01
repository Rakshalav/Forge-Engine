#pragma once

#include <stb-image/stb_image.h>

namespace Forge
{
	class Texture
	{
	public:
		unsigned int ID;
		Texture(const char* filename);
	};
}
