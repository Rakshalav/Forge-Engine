#pragma once

#include "Maths/Math.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Model.hpp"
#include "Renderer/Shader.hpp"

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
			glm::mat4 rotation		= glm::mat4_cast(glm::quat(Rotation));
			glm::mat4 scale			= glm::scale(glm::mat4(1.0f), Scale);
			return translation * rotation * scale;
		}
	};

	struct MeshComponent
	{
		Ref<Model> Model;
		Ref<Shader> Shader;

		MeshComponent() = default;
		MeshComponent(Ref<fg::Model> model, Ref<fg::Shader> shader) : Model(model), Shader(shader) {}
	};
}