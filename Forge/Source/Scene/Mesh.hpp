#pragma once

#include "../Maths/Math.hpp"
#include "../Renderer/Shader.hpp"
#include "../Renderer/Buffer.hpp"
#include "../Renderer/VertexArray.hpp"
#include "../Renderer/Texture.hpp"

#include <string>
#include <vector>

namespace fg
{
	struct Vertex
	{
		Vec3f Position;
		Vec3f Normal;
		Vec2f TextureCoords;
		Vec3f Tangent;
		Vec3f Bitangent;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Texture2D>>& textures);

		void Draw(Ref<Shader>& shader);

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexArray> m_VertexArray;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<Ref<Texture2D>> m_Textures;
	};
}