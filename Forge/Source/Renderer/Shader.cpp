#include <glad/glad.h>
#include "Shader.hpp"

#include <fstream>
#include <sstream>

namespace Forge
{
	Shader::Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
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
			//TODO: log error: File not succesfully read.
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
			//TODO: log error: Vertex shader compilation failed.
		};

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			//TODO: Fragment shader compilation failed.
		};

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			//TODO: Shader program linking failed.
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glDeleteProgram(ID); 
	}

	void Shader::use() const
	{
		glUseProgram(ID);
	}

	void Shader::applyUniforms() const
	{
		use();
		for (auto& [name, value] : m_uniforms)
		{
			std::visit([&](auto&& v) { uploadUniform(name, v); }, value);
		}
	}

	uint32_t Shader::getLocation(const std::string& name) const
	{
		if (m_locationCache.contains(name))
			return m_locationCache[name];

		GLint location = glGetUniformLocation(ID, name.c_str());
		m_locationCache[name] = location;
		return location;
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(getLocation(name), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(getLocation(name), value);
	}

	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(getLocation(name), value);
	}

	void Shader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(getLocation(name), 1, &value[0]);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(getLocation(name), 1, &value[0]);
	}

	void Shader::setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(getLocation(name), 1, &value[0]);
	}

	void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(getLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, &mat[0][0]);
	}
}
