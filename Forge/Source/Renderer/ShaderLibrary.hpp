#pragma once

#include "Shader.hpp"
#include <unordered_map>
#include <vector>

namespace fg
{
	class ShaderLibrary
	{
	public:
		ShaderLibrary() = default;
		~ShaderLibrary() { m_Shaders.clear(); }
		
		Ref<Shader> Load(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader);
		Ref<Shader> Load(const std::string& name, const char* vertexCode, const char* fragmentCode);
		inline Ref<Shader> GetShader(const std::string& name) { return Exists(name) ? m_Shaders[name] : nullptr; }

		inline bool Exists(const std::string& name) { return m_Shaders.contains(name); }

		void Reload(const std::string& name);

		std::vector<std::string> GetShaderNames();

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}