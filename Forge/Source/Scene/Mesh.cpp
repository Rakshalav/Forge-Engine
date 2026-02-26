#include "Mesh.hpp"
#include "Renderer/Renderer.hpp"
#include "../Platform/OpenGL/OpenGLShader.hpp"
#include "Model.hpp"
#include "Debug/Log.hpp"

namespace fg
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Ref<Texture2D>>& textures)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
	{
		m_VertexArray = VertexArray::Create();
		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), static_cast<uint32_t>(m_Vertices.size() * sizeof(Vertex)));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), static_cast<uint32_t>(m_Indices.size() * sizeof(uint32_t)));

		BufferLayout layout;
		layout.Push(3, ElementType::FLOAT);
		layout.Push(3, ElementType::FLOAT);
		layout.Push(2, ElementType::FLOAT);
		layout.Push(3, ElementType::FLOAT);
		layout.Push(3, ElementType::FLOAT);

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	void Mesh::Draw(Ref<Shader>& shader)
	{		
		uint32_t diffuseNr = 1, specularNr = 1, ambientNr = 1, normalNr = 1, emmisiveNr = 1;

		for (uint32_t i = 0; i < m_Textures.size(); i++)
		{
			const Texture2D& texture = *m_Textures[i].get();

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
				name = "texture_emmisive";
				number = std::to_string(emmisiveNr++);
				break;

			case TextureType::NORMAL:
				name = "texture_normal";
				number = std::to_string(normalNr++);
				break;
		
			default:
				FG_CORE_ERROR("Texture type ({}) not supported.", static_cast<uint8_t>(texture.GetType()));
				continue;
			}

			shader->SetInt(("material." + name + number).c_str(), i);
			texture.Bind();
		}

		Renderer::Submit(m_VertexArray, shader);
	}
}