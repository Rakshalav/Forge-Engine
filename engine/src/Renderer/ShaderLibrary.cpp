#include <Renderer/ShaderLibrary.hpp>

namespace fg
{
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader)
	{
		if (Exists(name))
			return nullptr;

		auto shader = Shader::Create(vertexShader, fragmentShader);
		m_Shaders[name] = shader;

		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const char* vertexCode, const char* fragmentCode)
	{
		if (Exists(name))
			return nullptr;

		auto shader = Shader::Create(vertexCode, fragmentCode);
		m_Shaders[name] = shader;

		return shader;
	}

	void ShaderLibrary::Reload(const std::string& name)
	{
		
	}

	std::vector<std::string> ShaderLibrary::GetShaderNames()
	{
		std::vector<std::string> names;
		names.reserve(m_Shaders.size());

		for (const auto& [name, shader] : m_Shaders)
			names.push_back(name);

		return names;
	}
}