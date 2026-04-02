#include "OpenGLCubemap.hpp"
#include <glad/glad.h>
#include <stb-image/stb_image.h>
#include "Debug/Log.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Maths/Math.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Renderer.hpp"

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

static const char* SkyboxVert = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    TexCoords = aPos;
    vec4 pos = u_Projection * mat4(mat3(u_View)) * vec4(aPos, 1.0);
    gl_Position = pos.xyww; 
}
)";

static const char* SkyboxFrag = R"(
#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_Skybox;

void main()
{    
    vec3 envColor = texture(u_Skybox, TexCoords).rgb;
    float exposure = 1.0; 
    envColor *= exposure;
    vec3 mapped = envColor / (envColor + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / 2.2)); 

    FragColor = vec4(mapped, 1.0);
}
)";

static const char* ConverterVert = R"(
#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    TexCoords = aPos;
    gl_Position = u_Projection * u_View * vec4(aPos, 1.0);
}
)";

static const char* ConverterFrag = R"(
#version 460 core
in vec3 TexCoords;
out vec4 FragColor;

uniform sampler2D u_EquirectMap;

const vec2 invAtan = vec2(0.1591f, 0.3183f); // (1/2PI, 1/PI)

void main()
{
    vec3 dir = normalize(TexCoords);
    vec2 uv = vec2(atan(dir.z, dir.x), asin(dir.y));
    
    uv *= invAtan;
    uv += 0.5f;
    
    FragColor = texture(u_EquirectMap, uv);
}
)";

namespace fg
{
	OpenGLCubemap::OpenGLCubemap(const std::string& path)
	{
		Renderer::GetShaderLibrary().Load("Skybox", SkyboxVert, SkyboxFrag);
		Renderer::GetShaderLibrary().Load("EquirectangularToCubemap", ConverterVert, ConverterFrag);

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
		glTextureStorage2D(m_RendererID, 1, GL_RGB32F, m_CubemapWidth, m_CubemapWidth);
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

		auto conversionShader = Renderer::GetShaderLibrary().GetShader("EquirectangularToCubemap");

		conversionShader->Bind();
		conversionShader->SetInt("u_EquirectMap", 0);
		conversionShader->SetMat4("u_Projection", projection);

		RenderCommand::Submit([textureID, width, FBO]() {
			glBindTextureUnit(0, textureID);
			glViewport(0, 0, width, width);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		});

		RenderCommand::Submit([vao]() { vao->Bind(); });

		for (uint32_t i = 0; i < 6; i++)
		{
			conversionShader->SetMat4("u_View", views[i]);

			RenderCommand::Submit([FBO, id, i]() {
				glNamedFramebufferTextureLayer(FBO, GL_COLOR_ATTACHMENT0, id, 0, i);

				auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (status != GL_FRAMEBUFFER_COMPLETE)
					FG_ERROR("FBO incomplete on face {}: {}", i, status);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			});
		}

		RenderCommand::Submit([FBO, RBO]() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &FBO);
			glDeleteRenderbuffers(1, &RBO);
		});
	}

	void OpenGLCubemap::Draw(Camera& camera)
	{
		auto shader = Renderer::GetShaderLibrary().GetShader("Skybox");

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