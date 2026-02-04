#pragma once

#include <glm/glm.hpp>

namespace fg
{
	using Vec2f = glm::vec2;
	using Vec3f = glm::vec3;
	using Vec4f = glm::vec4;

	template<glm::length_t size, typename T>
	using Vec = glm::vec<size, T, glm::qualifier::defaultp>;

	typedef Vec<2, int> Vec2i;
	typedef Vec<3, int> Vec3i;
	typedef Vec<4, int> Vec4i;

	typedef Vec<2, uint32_t> Vec2u;
	typedef Vec<3, uint32_t> Vec3u;
	typedef Vec<4, uint32_t> Vec4u;

	typedef Vec<2, double> Vec2d;
	typedef Vec<3, double> Vec3d;
	typedef Vec<4, double> Vec4d;
}