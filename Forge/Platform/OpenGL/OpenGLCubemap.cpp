#include "OpenGLCubemap.hpp"
#include <glad/glad.h>
#include <stb-image/stb_image.h>
#include "Debug/Log.hpp"
#include "Renderer/RenderCommand.hpp"

float SkyboxVertices[] = {
                              
	// Right (+X)        //   7--------6       
	 1.0f, -1.0f, -1.0f, //  /|       /|
	 1.0f, -1.0f,  1.0f, // 4--------5 |       
	 1.0f,  1.0f,  1.0f, // | |      | |
	 1.0f,  1.0f,  1.0f, // | 3------|-2
	 1.0f,  1.0f, -1.0f, // |/       |/
	 1.0f, -1.0f, -1.0f, // 0--------1
	                     
	 // Left (-X)
	 -1.0f, -1.0f,  1.0f,
	 -1.0f, -1.0f, -1.0f,
	 -1.0f,  1.0f, -1.0f,
	 -1.0f,  1.0f, -1.0f,
	 -1.0f,  1.0f,  1.0f,
	 -1.0f, -1.0f,  1.0f,

	 // Top (+Y)
	 -1.0f,  1.0f, -1.0f,
	  1.0f,  1.0f, -1.0f,
	  1.0f,  1.0f,  1.0f,
	  1.0f,  1.0f,  1.0f,
	 -1.0f,  1.0f,  1.0f,
	 -1.0f,  1.0f, -1.0f,

	 // Bottom (-Y)
	 -1.0f, -1.0f,  1.0f,
	  1.0f, -1.0f,  1.0f,
	  1.0f, -1.0f, -1.0f,
	  1.0f, -1.0f, -1.0f,
	 -1.0f, -1.0f, -1.0f,
	 -1.0f, -1.0f,  1.0f,

	 // Front (+Z)
	 -1.0f, -1.0f,  1.0f,
	 -1.0f,  1.0f,  1.0f,
	  1.0f,  1.0f,  1.0f,
	  1.0f,  1.0f,  1.0f,
	  1.0f, -1.0f,  1.0f,
	 -1.0f, -1.0f,  1.0f,

	 // Back (-Z)
	  1.0f, -1.0f, -1.0f,
	  1.0f,  1.0f, -1.0f,
	 -1.0f,  1.0f, -1.0f,
	 -1.0f,  1.0f, -1.0f,
	 -1.0f, -1.0f, -1.0f,
	  1.0f, -1.0f, -1.0f,
};

namespace fg
{
	OpenGLCubemap::OpenGLCubemap(std::array<std::string, 6> texturePaths) 
	{
		LoadCubemap(texturePaths);

		auto vertexBuffer = VertexBuffer::Create(SkyboxVertices, sizeof(SkyboxVertices));

		BufferLayout layout;
		layout.Push(3, ElementType::FLOAT);

		vertexBuffer->SetLayout(layout);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
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
				GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
				GLenum internalFormat = (nrChannels == 4) ? GL_RGBA8 : GL_RGB8;

				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, internalFormat, width, height, 0,
					format, GL_UNSIGNED_BYTE, data
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
		shader->Bind();
		shader->SetInt("u_Skybox", 0);
		shader->SetMat4("u_View", camera.GetViewMatrix());
		shader->SetMat4("u_Projection", camera.GetProjectionMatrix());

		auto id = m_RendererID;
		auto vertexArray = m_VertexArray;

		RenderCommand::Submit([id, vertexArray]()
			{
				glDepthFunc(GL_LEQUAL);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, id);
				vertexArray->Bind();
				glDrawArrays(GL_TRIANGLES, 0, 36); 
				glDepthFunc(GL_LESS);
			});
	}
}