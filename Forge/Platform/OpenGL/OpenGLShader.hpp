#pragma once

#include "../Source/Renderer/Shader.hpp"

#include <unordered_map>

namespace Forge
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexpath, const std::string& fragmentpath);
		~OpenGLShader();

		void Bind() const override;
		void UnBind() const override;

		template<typename T>
		void Set(const std::string& name, const T& value);

		void SetInt(const std::string& name, int value) override;
		void SetFloat(const std::string& name, float value) override;
		void SetBool(const std::string& name, bool value) override;

		void SetVec2(const std::string& name, const glm::vec2& value) override;
		void SetVec3(const std::string& name, const glm::vec3& value) override;
		void SetVec4(const std::string& name, const glm::vec4& value) override;

		void SetMat2(const std::string& name, const glm::mat2& value) override;
		void SetMat3(const std::string& name, const glm::mat3& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		template<typename T>
		void UploadUniform(const std::string& name, const T& value);

		void UploadUniformBool(const std::string& name, bool value);
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformFloat(const std::string& name, float value);

		void UploadUniformVec2(const std::string& name, const glm::vec2& value);
		void UploadUniformVec3(const std::string& name, const glm::vec3& value);
		void UploadUniformVec4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat2(const std::string& name, const glm::mat2& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& value);

	private:
		int GetLocation(const std::string& name) const;

	private:
		uint32_t m_RendererID;

		mutable std::unordered_map<std::string, int> m_LocationCache;
	};

	template<typename T>
	inline void OpenGLShader::Set(const std::string& name, const T& value)
	{
		static_assert(
			std::is_same_v<T, bool> || std::is_same_v<T, int> || std::is_same_v<T, float> ||
			std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec3> || std::is_same_v<T, glm::vec4> ||
			std::is_same_v<T, glm::mat2> || std::is_same_v<T, glm::mat3> || std::is_same_v<T, glm::mat4>,
			"Unsupported uniform type used in Shader::UploadUniform()"
			);

		if constexpr (std::is_same_v< T, bool>)
			SetBool(name, value);
		else if constexpr (std::is_same_v< T, int>)
			SetInt(name, value);
		else if constexpr (std::is_same_v< T, float>)
			SetFloat(name, value);
		else if constexpr (std::is_same_v< T, glm::vec2>)
			SetVec2(name, value);
		else if constexpr (std::is_same_v< T, glm::vec3>)
			SetVec3(name, value);
		else if constexpr (std::is_same_v< T, glm::vec4>)
			SetVec4(name, value);
		else if constexpr (std::is_same_v< T, glm::mat2>)
			SetMat2(name, value);
		else if constexpr (std::is_same_v< T, glm::mat3>)
			SetMat3(name, value);
		else if constexpr (std::is_same_v< T, glm::mat4>)
			SetMat4(name, value);
	}

	template<typename T>
	inline void OpenGLShader::UploadUniform(const std::string& name, const T& value)
	{
		static_assert(
			std::is_same_v<T, bool> || std::is_same_v<T, int> || std::is_same_v<T, float> ||
			std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec3> || std::is_same_v<T, glm::vec4> ||
			std::is_same_v<T, glm::mat2> || std::is_same_v<T, glm::mat3> || std::is_same_v<T, glm::mat4>,
			"Unsupported uniform type used in Shader::UploadUniform()"
			);

		if constexpr (std::is_same_v< T, bool>)
			UploadUniformBool(name, value);
		else if constexpr (std::is_same_v< T, int>)
			UploadUniformInt(name, value);
		else if constexpr (std::is_same_v< T, float>)
			UploadUniformFloat(name, value);
		else if constexpr (std::is_same_v< T, glm::vec2>)
			UploadUniformVec2(name, value);
		else if constexpr (std::is_same_v< T, glm::vec3>)
			UploadUniformVec3(name, value);
		else if constexpr (std::is_same_v< T, glm::vec4>)
			UploadUniformVec4(name, value);
		else if constexpr (std::is_same_v< T, glm::mat2>)
			UploadUniformMat2(name, value);
		else if constexpr (std::is_same_v< T, glm::mat3>)
			UploadUniformMat3(name, value);
		else if constexpr (std::is_same_v< T, glm::mat4>)
			UploadUniformMat4(name, value);
	}
}

