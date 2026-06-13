#include "ProjectWindow.hpp"

namespace Editor
{
	void ProjectWindow::OnRender()
	{
		ImGui::Begin("Project");
		{
			auto assetsDir = fg::Project::GetAssetDirectory();

			for (const auto& entry : std::filesystem::recursive_directory_iterator(assetsDir))
			{
				const auto& path = entry.path();
				std::string filename = path.filename().string();

				std::filesystem::path relativePath = std::filesystem::relative(path, assetsDir);

				if (entry.is_directory())
				{
					if (ImGui::TreeNode(filename.c_str()))
						ImGui::TreePop();
				}
				else
				{
					auto editorAssetManager = fg::Project::GetActive()->GetEditorAssetManager();
					auto handle = editorAssetManager->GetHandleFromRelativePath(relativePath);

					if (handle != 0)
					{
						auto type = editorAssetManager->GetAssetType(handle);
						ImGui::Text("%s", filename.c_str());
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
						ImGui::Text("%s", filename.c_str());
						ImGui::PopStyleColor();

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							editorAssetManager->RegisterAsset(relativePath);
							if (editorAssetManager->SerializeAssetRegistry())
								FG_INFO("{} imported succesfully!", filename.c_str());
						}
					}
				}

			}
		}
		ImGui::End();
	}
}