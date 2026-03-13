#pragma once

#include "../Source/Renderer/Shader.hpp"
#include <unordered_map>

namespace fg
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexpath, const std::string& fragmentpath);
		~OpenGLShader();

		void Bind() const override;
		void UnBind() const override;

		void SetInt(std::string name, int value) override;
		void SetFloat(std::string name, float value) override;
		void SetBool(std::string name, bool value) override;

		void SetVec2(std::string name, Vec2f value) override;
		void SetVec3(std::string name, Vec3f value) override;
		void SetVec4(std::string name, Vec4f value) override;

		void SetMat2(std::string name, glm::mat2 value) override;
		void SetMat3(std::string name, glm::mat3 value) override;
		void SetMat4(std::string name, glm::mat4 value) override;

	private:
		int GetLocation(const std::string& name);

	private:
		uint32_t m_RendererID;
		mutable std::unordered_map<std::string, int> m_LocationCache;
	};
}