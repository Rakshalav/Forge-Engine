#include "OpenGLCubemap.hpp"
#include <glad/glad.h>
#include <stb-image/stb_image.h>
#include "Debug/Log.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Maths/Math.hpp"
#include <glm/gtc/matrix_transform.hpp>

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
	OpenGLCubemap::OpenGLCubemap(const std::string& path, const Ref<Shader>& shader1, const Ref<Shader>& shader2) : m_CubemapShader(shader1), m_ConversionShader(shader2)
	{
		auto vertexBuffer = VertexBuffer::Create(SkyboxVertices, sizeof(SkyboxVertices));
		BufferLayout layout;
		layout.Push(3, ElementType::FLOAT);
		vertexBuffer->SetLayout(layout);
		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		LoadFile(path.c_str());
		CreateCubemap();
		ConvertEquirectangularImage();
	}

	void OpenGLCubemap::LoadFile(const char* path)
	{
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); 
		float* data = stbi_loadf(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			m_CubemapWidth = width / 4;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
			glTextureStorage2D(m_TextureID, 1, GL_RGB32F, width, height);
			glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureSubImage2D(m_TextureID, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, data);
		}
		else
			FG_ERROR("Failed to load enviroment map at path: {}", path);
		stbi_image_free(data);
	}

	void OpenGLCubemap::CreateCubemap()
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGB16F, m_CubemapWidth, m_CubemapWidth);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void OpenGLCubemap::ConvertEquirectangularImage()
	{
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		glm::mat4 views[] = {
			glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // +X
			glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // -X
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)), // +Y
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)), // -Y
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // +Z
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))  // -Z
		};

		uint32_t FBO, RBO;
		glCreateFramebuffers(1, &FBO);
		glCreateRenderbuffers(1, &RBO);
		glNamedRenderbufferStorage(RBO, GL_DEPTH_COMPONENT24, m_CubemapWidth, m_CubemapWidth);
		glNamedFramebufferRenderbuffer(FBO, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

		auto textureID = m_TextureID;
		auto width = m_CubemapWidth;
		auto id = m_RendererID;
		auto& vao = m_VertexArray;

		m_ConversionShader->Bind();
		m_ConversionShader->SetInt("u_EquirectMap", 0);
		m_ConversionShader->SetMat4("u_Projection", projection);

		RenderCommand::Submit([textureID, width, FBO]() {
			glBindTextureUnit(0, textureID);
			glViewport(0, 0, width, width);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		});

		for (uint32_t i = 0; i < 6; i++)
			m_ConversionShader->SetMat4("u_View", views[i]);

		RenderCommand::Submit([FBO, RBO, id, vao]() {
			vao->Bind();
			for (uint32_t i = 0; i < 6; i++)
			{
				glNamedFramebufferTextureLayer(FBO, GL_COLOR_ATTACHMENT0, id, 0, i);

				auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (status != GL_FRAMEBUFFER_COMPLETE)
				{
					FG_ERROR("FBO incomplete on face {}: {}", i, status);
					continue;
				}

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &FBO);
			glDeleteRenderbuffers(1, &RBO);
		});
	}

	void OpenGLCubemap::Draw(Camera& camera)
	{
		m_CubemapShader->Bind();
		m_CubemapShader->SetInt("u_Skybox", 0);
		m_CubemapShader->SetMat4("u_View", camera.GetViewMatrix());
		m_CubemapShader->SetMat4("u_Projection", camera.GetProjectionMatrix());

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