#include <glad/glad.h>
#include "OpenGLShader.hpp"
#include <fstream>
#include <sstream>
#include "Debug/Log.hpp"
#include "Renderer/RenderCommand.hpp"

namespace fg
{
	OpenGLShader::OpenGLShader(const std::string& vertexpath, const std::string& fragmentpath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexpath);
			fShaderFile.open(fragmentpath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			FG_ERROR("File not succesfully read | Vertex Path: {} | Fragment Path: {}", vertexpath, fragmentpath);
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			FG_ERROR("Vertex shader compilation failed.");
		};

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			FG_ERROR("Fragment shader compilation failed.");
		};

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertex);
		glAttachShader(m_RendererID, fragment);
		glLinkProgram(m_RendererID);

		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
			FG_ERROR("Shader program linking failed.");
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	OpenGLShader::~OpenGLShader()
	{
		m_LocationCache.clear();
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		auto id = m_RendererID;
		RenderCommand::Submit([id]() { glUseProgram(id); });
	}

	void OpenGLShader::UnBind() const
	{
		RenderCommand::Submit([]() { glUseProgram(0); });
	}

	void OpenGLShader::SetInt(std::string name, int value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniform1i(GetLocation(name), value); });
	}

	void OpenGLShader::SetFloat(std::string name, float value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniform1f(GetLocation(name), value); });
	}

	void OpenGLShader::SetBool(std::string name, bool value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniform1i(GetLocation(name), (int)value); });
	}

	void OpenGLShader::SetVec2(std::string name, Vec2f value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniform2fv(GetLocation(name), 1, &value[0]); });
	}

	void OpenGLShader::SetVec3(std::string name, Vec3f value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniform3fv(GetLocation(name), 1, &value[0]); });
	}

	void OpenGLShader::SetVec4(std::string name, Vec4f value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniform4fv(GetLocation(name), 1, &value[0]); });
	}

	void OpenGLShader::SetMat2(std::string name, glm::mat2 value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniformMatrix2fv(GetLocation(name), 1, GL_FALSE, &value[0][0]); });
	}

	void OpenGLShader::SetMat3(std::string name, glm::mat3 value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, &value[0][0]); });
	}

	void OpenGLShader::SetMat4(std::string name, glm::mat4 value)
	{
		RenderCommand::Submit([this, name = std::move(name), value]() { glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &value[0][0]); });
	}

	int OpenGLShader::GetLocation(const std::string& name) 
	{
		if (m_LocationCache.contains(name))
			return m_LocationCache[name];

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_LocationCache.emplace(name, location);
		return location;
	}
}