#include <glad/glad.h>

#include "Mesh.hpp"
#include "Renderer/Renderer.hpp"
#include "../Platform/OpenGL/OpenGLShader.hpp"
#include "Model.hpp"
#include "Debug/Log.hpp"

namespace fg
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
	{
		this->Vertices = vertices;
		this->Indices = indices;
		this->Textures = textures;

		SetupMesh();
	}

	void Mesh::Draw(Ref<Shader>& shader)
	{		
		uint32_t diffuseNr = 1, specularNr = 1;

		for (uint32_t i = 0; i < Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = Textures[i].Type;

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			auto& Shader = *shader.get();
			Shader.SetInt(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, Textures[i].ID);
		}

		Renderer::Draw(m_VertexArray, shader);
		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::SetupMesh()
	{
		m_VertexArray = VertexArray::Create();
		m_VertexBuffer = VertexBuffer::Create((float*)Vertices.data(), static_cast<uint32_t>(Vertices.size() * sizeof(Vertex)));
		m_IndexBuffer = IndexBuffer::Create(Indices.data(), (uint32_t)Indices.size());

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
}