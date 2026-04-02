#pragma once

#include "Maths/Math.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Mesh.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Material.hpp"

namespace fg
{
	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Vec3f& translation) : Translation(translation) {}
		
		Vec3f Translation	= { 0.0f, 0.0f, 0.0f };
		Vec3f Rotation		= { 0.0f, 0.0f, 0.0f };
		Vec3f Scale			= { 1.0f, 1.0f, 1.0f };

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
		MeshComponent() = default;
		MeshComponent(Ref<fg::Mesh> mesh) : Mesh(mesh) {}

		Ref<Mesh> Mesh;
	};

	struct MeshRendererComponent
	{
		MeshRendererComponent() = default;

		std::vector<Ref<Material>> Materials;

		enum ShadowFlag : uint8_t
		{
			None			= 0 << 0,
			CastShadow		= 1 << 0,
			TwoSided		= 1 << 1,
			ReceiveShadow	= 1 << 2
		};

		uint8_t ShadowFlags = CastShadow | ReceiveShadow;
	};
}