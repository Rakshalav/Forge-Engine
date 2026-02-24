#pragma once

#include "../Maths/Math.hpp"
#include "../Renderer/Shader.hpp"
#include "../Renderer/Buffer.hpp"
#include "../Renderer/VertexArray.hpp"

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

	struct Texture
	{
		uint32_t ID;
		std::string Type;
		std::string Path;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<Texture> Textures;

		void Draw(Ref<Shader>& shader);

	private:
		void SetupMesh();

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexArray> m_VertexArray;
	};
}