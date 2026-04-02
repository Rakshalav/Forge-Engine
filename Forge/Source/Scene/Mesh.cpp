#include "Mesh.hpp"
#include "Renderer/Renderer.hpp"
#include "../Platform/OpenGL/OpenGLShader.hpp"
#include "Debug/Log.hpp"

namespace fg
{
	SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		: Vertices(vertices), Indices(indices)
	{
		VertexArray = VertexArray::Create();
		auto vertexBuffer = VertexBuffer::Create((float*)Vertices.data(), static_cast<uint32_t>(Vertices.size() * sizeof(Vertex)));
		auto indexBuffer = IndexBuffer::Create(Indices.data(), static_cast<uint32_t>(Indices.size() * sizeof(uint32_t)));

		BufferLayout layout;
		layout.Push(3, ElementType::FLOAT);
		layout.Push(3, ElementType::FLOAT);
		layout.Push(2, ElementType::FLOAT);
		layout.Push(3, ElementType::FLOAT);
		layout.Push(3, ElementType::FLOAT);

		vertexBuffer->SetLayout(layout);
		VertexArray->AddVertexBuffer(vertexBuffer);
		VertexArray->SetIndexBuffer(indexBuffer);
	}

/*	void SubMesh::Draw()
	{		
		uint32_t diffuseNr = 1, specularNr = 1, ambientNr = 1, normalNr = 1, emissiveNr = 1;

		for (uint32_t i = 0; i < Textures.size(); i++)
		{
			const Texture2D& texture = *Textures[i].get();

			texture.Activate(i);

			std::string number;
			std::string name;

			switch (texture.GetType())
			{
			case TextureType::DIFFUSE:
				name = "texture_diffuse";
				number = std::to_string(diffuseNr++);
				break;

			case TextureType::SPECULAR:
				name = "texture_specular";
				number = std::to_string(specularNr++);
				break;

			case TextureType::AMBIENT:
				name = "texture_ambient";
				number = std::to_string(ambientNr++);
				break;

			case TextureType::EMMISIVE:
				name = "texture_emissive";
				number = std::to_string(emissiveNr++);
				break;

			case TextureType::NORMAL:
				name = "texture_normal";
				number = std::to_string(normalNr++);
				break;
		
			default:
				FG_CORE_ERROR("Texture type ({}) not supported.", static_cast<uint8_t>(texture.GetType()));
				continue;
			}

			Renderer::GetShaderLibrary().GetShader("Model")->SetInt(("material." + name + number), i);
			texture.Bind();
		}

		Renderer::Submit(VertexArray);
	}*/
}