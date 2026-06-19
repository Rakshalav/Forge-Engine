#include "ProjectWindow.hpp"


namespace Editor
{
    ProjectWindow::ProjectWindow()
    {
        if (fg::Project::GetActive())
        {
            m_AssetManager = fg::Project::GetActive()->GetEditorAssetManager();
            m_RootDirectory = fg::Project::GetAssetDirectory();
            m_CurrentDirectory = m_RootDirectory;
        }
    }

    void ProjectWindow::OnRender()
    {
        ImGui::Begin("Project");
        {
            if (ImGui::BeginTable("ProjectWindowLayout", 2, ImGuiTableFlags_Resizable))
            {
                ImGui::TableSetupColumn("TreeView", ImGuiTableColumnFlags_WidthFixed, 200.0f);
                ImGui::TableSetupColumn("GridView", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::BeginChild("LeftTreeChild");
                {
                    RenderTreeLevel(m_RootDirectory);
                }
                ImGui::EndChild();

                ImGui::TableSetColumnIndex(1);
                ImGui::BeginChild("RightGridChild");
                {

                }
                ImGui::EndChild();

                ImGui::EndTable();
            }
        }
        ImGui::End();
    }

    void ProjectWindow::RenderTreeLevel(const std::filesystem::path& directoryPath)
    {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
        {
            const auto& path = entry.path();
            std::string filename = path.filename().string();
            auto relativePath = std::filesystem::relative(path, m_RootDirectory);

            std::string uniqueID = filename + "##" + relativePath.string();

            if (entry.is_directory())
            {
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

                if (m_CurrentDirectory == path)
                    flags |= ImGuiTreeNodeFlags_Selected;

                bool isNodeOpen = ImGui::TreeNodeEx(uniqueID.c_str(), flags, "%s", filename.c_str());

                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    m_CurrentDirectory = path; 
                    FG_INFO("current directory: {}", m_CurrentDirectory.string());
                }

                RenderFolderContextMenu(path);

                if (isNodeOpen)
                {
                    RenderTreeLevel(path);
                    ImGui::TreePop();
                }
            }
            else
            {
                auto handle = m_AssetManager->GetHandleFromRelativePath(relativePath);

                bool isSelected = (m_SelectedAssetPath == path);

                bool isImported = (handle != 0);
                if (!isImported)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

                if (ImGui::Selectable(uniqueID.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
                    m_SelectedAssetPath = path;
             
                if (!isImported)
                    ImGui::PopStyleColor();

                RenderAssetContextMenu(path, relativePath);
            }
        }
    }

    void ProjectWindow::RenderFolderContextMenu(const std::filesystem::path& folderPath)
    {
        if (ImGui::BeginPopupContextItem())
        {
            m_CurrentDirectory = folderPath;

            ImGui::TextDisabled("Folder Options");
            ImGui::Separator();
            if (ImGui::MenuItem("New Folder")) { /* TODO */ }
            if (ImGui::MenuItem("Delete Folder")) { 
                std::error_code ec;
                std::filesystem::remove_all(folderPath, ec);

                if (ec)
                    std::cerr << "Failed to delete folder: " << ec.message() << "\n";
            }

            ImGui::EndPopup();
        }
    }

    void ProjectWindow::RenderAssetContextMenu(const std::filesystem::path& assetPath, const std::filesystem::path& relativePath)
    {
        if (ImGui::BeginPopupContextItem())
        {
            m_SelectedAssetPath = assetPath;

            ImGui::TextDisabled("Asset Options");
            ImGui::Separator();

            auto handle = m_AssetManager->GetHandleFromRelativePath(relativePath);

            if (handle == 0)
            {
                if (ImGui::MenuItem("Import")) 
                {
                    m_AssetManager->RegisterAsset(relativePath);
                    m_AssetManager->SerializeAssetRegistry();
                }
            }
            else
            {
                if (ImGui::MenuItem("Exclude"))
                {
                    m_AssetManager->UnRegisterAsset(handle);
                    m_AssetManager->SerializeAssetRegistry();
                }
            }


            if (ImGui::MenuItem("Delete")) { std::filesystem::remove(assetPath); }
            if (ImGui::MenuItem("Copy File Path")) { fg::Utility::CopyToClipBoard(assetPath.string()); }

            ImGui::EndPopup();
        }
    }
}
