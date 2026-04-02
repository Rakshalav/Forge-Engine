#pragma once

#include <string>
#include <filesystem>
#include "Core/Base.hpp"

namespace fg
{
	struct ProjectConfig
	{
		std::string Name = "Untitled";
		std::filesystem::path StartScene;
		std::filesystem::path AssetDirectory;
	};

	class Project
	{
	public:
		static const std::filesystem::path& GetProjectDirectory()
		{
			if (s_ActiveProject)
				return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory()
		{
			if (s_ActiveProject)
				return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		static inline Ref<Project> s_ActiveProject = nullptr;
	};
}