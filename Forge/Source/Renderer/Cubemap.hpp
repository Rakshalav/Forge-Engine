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
		virtual void Draw(Camera& camera) = 0;

		static Ref<Cubemap> Create(const std::string& path, const Ref<Shader>& shader1, const Ref<Shader>& shader2);
	};
}