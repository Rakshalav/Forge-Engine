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

	struct SubMesh
	{
		SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		Ref<VertexArray> VertexArray;
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
	};

	struct Mesh : public Asset
	{
		std::vector<SubMesh> SubMeshes;

		AssetType GetAssetType() { return AssetType::Mesh; }
		AssetType GetAssetStaticType() { return AssetType::Mesh; }
	};
}