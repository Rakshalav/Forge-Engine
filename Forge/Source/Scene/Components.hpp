#pragma once

#include "Maths/Math.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace fg
{
	struct TransformComponent
	{
		Vec3f Translation	= { 0.0f, 0.0f, 0.0f };
		Vec3f Rotation		= { 0.0f, 0.0f, 0.0f };
		Vec3f Scale			= { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Vec3f& translation) : Translation(translation) {}

		inline glm::mat4 GetTransformation() const
		{
			glm::mat4 translation	= glm::translate(glm::mat4(1.0f), Translation);
			glm::mat4 rotation		= glm::toMat4(glm::quat(Rotation));
			glm::mat4 scale			= glm::scale(glm::mat4(1.0f), Scale);
			return translation * rotation * scale;
		}
	};
}