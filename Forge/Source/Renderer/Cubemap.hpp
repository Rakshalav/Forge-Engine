#pragma once

#include "Core/Base.hpp"
#include "Shader.hpp"
#include "Renderer/Camera.hpp"
#include <string>
#include <array>

namespace fg
{
	class Cubemap
	{
	public:
		virtual void LoadCubemap(std::array<std::string, 6>& texturePaths) = 0;
		virtual void Draw(Ref<Shader>& shader, Camera& camera) = 0;

		static Ref<Cubemap> Create(std::array<std::string, 6>& texturePaths);
	};
}