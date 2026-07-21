#pragma once

#include <string>
#include <filesystem>
#include "Core/Memory.hpp"
#include "AssetManager/AssetManagerEditor.hpp"

namespace fg
{
	struct ProjectConfig
	{
		std::string Name = "Untitled";
		std::filesystem::path AssetDirectory = "Assets";
		std::filesystem::path AssetRegistry = "Asset.db";
		std::filesystem::path StartScene = AssetDirectory / ("Scenes/" + Name + ".forge");
	};

	class Project : public RefCounted
	{
	public:
		static const std::filesystem::path& GetProjectDirectory()
		{
			if (s_ActiveProject)
				return s_ActiveProject->m_ProjectDirectory;
			return {};
		}

		static std::filesystem::path GetAssetDirectory()
		{
			if (s_ActiveProject)
				return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
			return {};
		}

		static std::filesystem::path GetAssetRegistryPath()
		{
			if (s_ActiveProject)
				return GetProjectDirectory() / s_ActiveProject->m_Config.AssetRegistry;
			return {};
		}

		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }

		Ref<AssetManagerBase> GetAssetManager() { return m_AssetManager; }
		Ref<AssetManagerEditor> GetEditorAssetManager() { return StaticRefCast<AssetManagerEditor>(m_AssetManager); }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool Create(const std::string& name, const std::filesystem::path& projectDirectory);
		static bool SaveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		Ref<AssetManagerBase> m_AssetManager;
		static inline Ref<Project> s_ActiveProject = nullptr;
	};
}