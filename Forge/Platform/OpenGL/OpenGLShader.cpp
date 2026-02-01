#include <glad/glad.h>

#include "OpenGLShader.hpp"

#include <fstream>
#include <sstream>

#include "../Source/Debug/Log.hpp"

namespace Forge
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
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		m_LocationCache.clear();
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetBool(const std::string& name, bool value)
	{
		UploadUniformBool(name, value);
	}

	void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformVec2(name, value);
	}

	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformVec3(name, value);
	}

	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformVec4(name, value);
	}

	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& value)
	{
		UploadUniformMat2(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		UploadUniformMat3(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformBool(const std::string& name, bool value)
	{
		glUniform1i(GetLocation(name), (int)value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		glUniform1i(GetLocation(name), value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		glUniform1f(GetLocation(name), value);
	}

	void OpenGLShader::UploadUniformVec2(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(GetLocation(name), 1, &value[0]);
	}

	void OpenGLShader::UploadUniformVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(GetLocation(name), 1, &value[0]);
	}

	void OpenGLShader::UploadUniformVec4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(GetLocation(name), 1, &value[0]);
	}

	void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& value)
	{
		glUniformMatrix2fv(GetLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	int OpenGLShader::GetLocation(const std::string& name) const
	{
		if (m_LocationCache.contains(name))
			return m_LocationCache[name];

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_LocationCache.emplace(name, location);
		return location;
	}
}