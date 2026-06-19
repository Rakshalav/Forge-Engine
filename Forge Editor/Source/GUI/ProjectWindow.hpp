#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <Forge.hpp>
	
namespace Editor
{
	class ProjectWindow
	{
	public:
		ProjectWindow();

		void OnEvent(fg::Event& event);
		void OnUpdate(float dt);
		void OnRender();

	private:
		void RenderTreeLevel(const std::filesystem::path& directoryPath);

		void RenderFolderContextMenu(const std::filesystem::path& folderPath);
		void RenderAssetContextMenu(const std::filesystem::path& assetPath, const std::filesystem::path& relativePath);

	private:
		std::filesystem::path m_RootDirectory;        
		std::filesystem::path m_CurrentDirectory;   
		std::filesystem::path m_SelectedAssetPath;

		fg::Ref<fg::AssetManagerEditor> m_AssetManager;
	};
}
