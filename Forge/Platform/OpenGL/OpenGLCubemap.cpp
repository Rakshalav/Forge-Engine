#include "OpenGLCubemap.hpp"
#include <glad/glad.h>
#include <stb-image/stb_image.h>
#include "Debug/Log.hpp"
#include "Renderer/RenderCommand.hpp"

float SkyboxVertices[] = {
	-1.0f, -1.0f, -1.0f,//   7--------6
	 1.0f, -1.0f, -1.0f,//  /|       /| 
	 1.0f, -1.0f,  1.0f,// 4--------5 |
	-1.0f, -1.0f,  1.0f,// | |      | | 
	-1.0f,  1.0f, -1.0f,// | 3------|-2
	 1.0f,  1.0f, -1.0f,// |/       |/
	 1.0f,  1.0f,  1.0f,// 0--------1
	-1.0f,  1.0f,  1.0f,
};

uint32_t SkyBoxIndices[] = {
	// Right (+X): 1, 5, 6, 2
	1, 5, 6,  6, 2, 1,
	// Left  (-X): 4, 0, 3, 7
	4, 0, 3,  3, 7, 4,
	// Top   (+Y): 4, 7, 6, 5
	4, 7, 6,  6, 5, 4,
	// Bottom(-Y): 0, 1, 2, 3
	0, 1, 2,  2, 3, 0,
	// Front (+Z): 3, 2, 6, 7
	3, 2, 6,  6, 7, 3,
	// Back  (-Z): 0, 4, 5, 1
	0, 4, 5,  5, 1, 0
};

namespace fg
{
	OpenGLCubemap::OpenGLCubemap(std::array<std::string, 6> texturePaths) 
	{
		LoadCubemap(texturePaths);

		auto vertexBuffer = VertexBuffer::Create(SkyboxVertices, sizeof(SkyboxVertices));
		auto indexBuffer = IndexBuffer::Create(SkyBoxIndices, sizeof(SkyBoxIndices));

		BufferLayout layout;
		layout.Push(3, ElementType::FLOAT);

		vertexBuffer->SetLayout(layout);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void OpenGLCubemap::LoadCubemap(std::array<std::string, 6>& texturePaths)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		int width, height, nrChannels;
		for (uint32_t i = 0; i < texturePaths.size(); i++)
		{
			unsigned char* data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
					0, GL_RGBA8, width, height, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				FG_ERROR("Cubemap texture failed to load at path: {}", texturePaths[i]);
				stbi_image_free(data);
			}
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void OpenGLCubemap::Draw(Ref<Shader>& shader, Camera& camera)
	{

		RenderCommand::SetDepthFunc(GL_LEQUAL);
		
		shader->Bind();
		auto id = m_RendererID;
		RenderCommand::Submit([id]() { 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, id); 
			});
		shader->SetInt("u_Skybox", 0);
		shader->SetMat4("u_View", camera.GetViewMatrix());
		shader->SetMat4("u_Projection", camera.GetProjectionMatrix());

		RenderCommand::DrawIndexed(m_VertexArray);

		RenderCommand::SetDepthFunc(GL_LESS);
	}
}