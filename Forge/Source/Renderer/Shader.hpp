#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <variant>
#include <unordered_map>
#include <tuple>

namespace Forge
{
	typedef std::variant<bool, float, int, glm::vec2, glm::vec3, glm::vec4, glm::mat2, glm::mat3, glm::mat4> uniform_type;

	struct Material
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;

		float Shininess;
	};

	struct Light
	{
		glm::vec3 position;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	class Shader {

	public:
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath);
		~Shader() { glDeleteProgram(ID); }

		inline void use() const { glUseProgram(ID); }

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;

		void setVec2(const std::string& name, const glm::vec2& value) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec4(const std::string& name, const glm::vec4& value) const;

		void setMat2(const std::string& name, const glm::mat2& mat) const;
		void setMat3(const std::string& name, const glm::mat3& mat) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;

		template<typename T>
		void set(const std::string& name, const T& value);

		inline const std::unordered_map<std::string, uniform_type>& getUniforms() const { return m_uniforms; }

		void applyUniforms();

	private:
		GLint getLocation(const std::string& name) const;

		template<typename T>
		void uploadUniform(const std::string& name, const T& value);

		mutable std::unordered_map<std::string, GLint> m_locationCache;
		std::unordered_map<std::string, uniform_type> m_uniforms;
	};

	template<typename T>
	inline void Shader::set(const std::string& name, const T& value)
	{
		static_assert(
			std::is_same_v<T, bool> ||
			std::is_same_v<T, int> ||
			std::is_same_v<T, float> ||
			std::is_same_v<T, glm::vec2> ||
			std::is_same_v<T, glm::vec3> ||
			std::is_same_v<T, glm::vec4> ||
			std::is_same_v<T, glm::mat2> ||
			std::is_same_v<T, glm::mat3> ||
			std::is_same_v<T, glm::mat4>,
			"Unsupported uniform type used in Shader::set()"
			);

		m_uniforms[name] = value;
	}

	template<typename T>
	inline void Shader::uploadUniform(const std::string& name, const T& value)
	{
		if constexpr (std::is_same_v<T, bool>)
			setBool(name, value);
		else if constexpr (std::is_same_v<T, int>)
			setInt(name, value);
		else if constexpr (std::is_same_v<T, float>)
			setFloat(name, value);
		else if constexpr (std::is_same_v<T, glm::vec2>)
			setVec2(name, value);
		else if constexpr (std::is_same_v<T, glm::vec3>)
			setVec3(name, value);
		else if constexpr (std::is_same_v<T, glm::vec4>)
			setVec4(name, value);
		else if constexpr (std::is_same_v<T, glm::mat2>)
			setMat2(name, value);
		else if constexpr (std::is_same_v<T, glm::mat3>)
			setMat3(name, value);
		else if constexpr (std::is_same_v<T, glm::mat4>)
			setMat4(name, value);
	}
}