#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "ProjectWindow.hpp"
#include <functional>
#include <fstream>

namespace Editor
{
    enum ShortCuts : uint8_t
    {
        ShortCut_Cut = 0,
        ShortCut_Copy,
        ShortCut_Paste,
        ShortCut_Delete,
        ShortCut_Rename,
        ShortCut_AddFile,
        ShortCut_AddFolder,

        ShortCut_Count
    };

    static const char* s_ShortCuts[ShortCut_Count] = {
        "Crl+X", "Crl+C", "Crl+V", "Del", "F2", "Crl+Shift+A", "Shift+Alt+A"
    };

    ContentBrowser::ContentBrowser()
    {
        if (fg::Project::GetActive())
        {
            m_AssetManager = fg::Project::GetActive()->GetEditorAssetManager();
            m_RootDirectory = fg::Project::GetAssetDirectory();

            m_FileWatcher = new efsw::FileWatcher();
            m_UpdateListener = new fg::UpdateListener();

            efsw::WatchID watchID = m_FileWatcher->addWatch(m_RootDirectory.generic_string(), m_UpdateListener, true);
            if (watchID < efsw::Errors::NoError)
                FG_ERROR("Failed to watch asset directory: {}", efsw::Errors::Log::getLastErrorLog());
            else
                m_FileWatcher->watch();

            m_AssetTree.BuildTreeFromDir(m_RootDirectory);

            m_AssetTree.CurrentDirectory = m_AssetTree.GetRoot();
        }
    }

    void ContentBrowser::OnEvent(fg::Event& event)
    {
        fg::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<fg::Event::FileChange>([this](fg::Event::FileChange& e) { return m_AssetTree.OnFileChange(e); });
    }

    static bool showDeleteDialog = false;
    static bool dontAskNextTime = false;

    static bool showNewItemModal = false;

    void ContentBrowser::OnRender()
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
                    HierarchyPanel();
                }
                ImGui::EndChild();

                ImGui::TableSetColumnIndex(1);
                ImGui::BeginChild("RightGridChild");
                {
                    RenderGridView(m_AssetTree.CurrentDirectory);
                }
                ImGui::EndChild();

                ImGui::EndTable();
            }
        }
        ImGui::End();

        if (!dontAskNextTime && showDeleteDialog && !ImGui::IsPopupOpen("Delete?"))
            ImGui::OpenPopup("Delete?");

        if (showNewItemModal && !ImGui::IsPopupOpen("Add New Item"))
            ImGui::OpenPopup("Add New Item");

        RenderDeleteModal();
        RenderAddFileModal();
    }

    void ContentBrowser::HierarchyPanel()
    {
        if (!m_AssetTree.GetRoot())
            return;

        auto selectNode = [this](AssetNode* node)
        {
            m_AssetTree.SelectedNode = node;
            m_AssetTree.CurrentDirectory = node->isDirectory ? node : node->Parent;
        };

        std::function<void(AssetNode*)> drawTree = [&](AssetNode* parent)
        {
            for (const auto& child : parent->Children)
            {
                if (!m_PathToRenameOnCreation.empty() && child->Path == m_PathToRenameOnCreation)
                {
                    m_RenamingNode = child.get();
                    strncpy_s(m_RenameBuffer, child->Name.c_str(), _TRUNCATE);
                    m_PathToRenameOnCreation.clear();
                }

                if (m_RenamingNode == child.get())
                {
                    RenameFileOrFolder();
                    continue;
                }

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
                if (m_AssetTree.SelectedNode == child.get())
                    flags |= ImGuiTreeNodeFlags_Selected;

                if (child->isDirectory)
                    flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DrawLinesNone;
                else
                    flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

                const bool isOpen = ImGui::TreeNodeEx((void*)(uintptr_t)child->Id, flags, "%s", child->Name.c_str());
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
                    selectNode(child.get());

                RenderContextMenu(child.get());

                if (child->isDirectory && isOpen)
                {
                    drawTree(child.get());
                    ImGui::TreePop();
                }
            }
        };

        AssetNode* root = m_AssetTree.GetRoot();
        ImGuiTreeNodeFlags rootNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesNone | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
        if (m_AssetTree.SelectedNode == root)
            rootNodeFlags |= ImGuiTreeNodeFlags_Selected;
        const bool rootOpen = ImGui::TreeNodeEx((void*)(uintptr_t)root->Id, rootNodeFlags, "%s", root->Name.c_str());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
            selectNode(root);

        RenderContextMenu(root);
      
        if (rootOpen)
        {
            drawTree(root);
            ImGui::TreePop();
        }

        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && m_AssetTree.SelectedNode && m_AssetTree.SelectedNode != root)
        {
            if (ImGui::IsKeyPressed(ImGuiKey_F2))
            {
                m_RenamingNode = m_AssetTree.SelectedNode;
                strncpy_s(m_RenameBuffer, m_RenamingNode->Name.c_str(), _TRUNCATE);
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_Delete))
            {
                m_TargetDeletePath = m_AssetTree.SelectedNode->Path;
                if (dontAskNextTime)
                    DeleteFileOrFolder();
                else
                    showDeleteDialog = true;
            }
        }
    }

    void ContentBrowser::RenameFileOrFolder()
    {
        ImGui::SetKeyboardFocusHere();

        float textWidth = ImGui::CalcTextSize(m_RenameBuffer).x;
        float extraWidthPadding = 10.0f;
        ImGui::SetNextItemWidth(textWidth + extraWidthPadding);
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, style.FramePadding.y));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

        const bool renameSubmitted = ImGui::InputText("##RenameBox", m_RenameBuffer, IM_ARRAYSIZE(m_RenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
        if (renameSubmitted)
        {
            if (strlen(m_RenameBuffer) > 0 && m_RenameBuffer != m_RenamingNode->Name)
            {
                std::filesystem::path oldPath = m_RenamingNode->Path;
                std::filesystem::path newPath = oldPath.parent_path() / m_RenameBuffer;

                try {
                    std::filesystem::rename(oldPath, newPath);
                }
                catch (const std::filesystem::filesystem_error& e) {
                    FG_ERROR("Failed to rename: {}", e.what());
                }
            }
            m_RenamingNode = nullptr;
        }

        ImGui::PopStyleVar(3);

        if (!renameSubmitted && (ImGui::IsItemDeactivated() || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))))
            m_RenamingNode = nullptr;
    }

    void ContentBrowser::RenderGridView(AssetNode* currentDirectory)
    {
        //auto& children = currentDirectory->Children;

        //static float iconSize = 64.0f;
        //static float padding = 16.0f;
        //float cellSize = iconSize + padding;

        //float panelWidth = ImGui::GetContentRegionAvail().x;
        //int columnCount = (int)(panelWidth / cellSize);
        //if (columnCount < 1)
        //    columnCount = 1;

        //ImGui::Columns(columnCount, nullptr, false);

        //for (const auto& childPtr : children)
        //{
        //    AssetNode* node = childPtr.get();
        //    ImGui::PushID((void*)(uintptr_t)node->Id);

        //    bool isSelected = (m_AssetTree.SelectedNode == node);

        //    //TODO: Icon button (swap for a texture later via ImGui::ImageButton)
        //    if (isSelected)
        //        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);

        //    if (ImGui::Button("##icon", ImVec2(iconSize, iconSize)))
        //        m_AssetTree.SelectedNode = node;

        //    if (isSelected)
        //        ImGui::PopStyleColor();

        //    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        //    {
        //        if (node->isDirectory)
        //            m_AssetTree.CurrentDirectory = node;
        //    }

        //    RenderContextMenu(node);

        //    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + iconSize);
        //    ImGui::TextWrapped("%s", node->Name.c_str());
        //    ImGui::PopTextWrapPos();

        //    ImGui::PopID();
        //    ImGui::NextColumn();
        //}

        //ImGui::Columns(1);
    }

    void ContentBrowser::RenderContextMenu(AssetNode* node)
    {
        if (ImGui::BeginPopupContextItem())
        {
            std::filesystem::path fullPath = node->Path;

            auto commonButtons = [&]() {
                bool isCurrentRoot = (node == m_AssetTree.GetRoot());
                bool isCurrentDir = node->isDirectory;

                if (!isCurrentRoot)
                {
                    if (ImGui::MenuItem("Cut", s_ShortCuts[ShortCut_Cut])) {}

                    if (ImGui::MenuItem("Copy", s_ShortCuts[ShortCut_Copy]))
                    {
                        m_CopiedPaths = { node->Path };
                        FG_INFO("{} copied!", node->Name);
                    }
                }

                if (isCurrentDir)
                {
                    if (ImGui::MenuItem("Paste", s_ShortCuts[ShortCut_Paste], false, !m_CopiedPaths.empty()))
                    {
                        for (const auto& sourcePath : m_CopiedPaths)
                        {
                            const std::filesystem::path destinationPath = fullPath / sourcePath.filename();
                            const std::filesystem::path relativeDestination = destinationPath.lexically_relative(sourcePath);
                            const bool destinationIsInsideSource = !relativeDestination.empty() && *relativeDestination.begin() != "..";
                            if (sourcePath == destinationPath || destinationIsInsideSource || std::filesystem::exists(destinationPath))
                                continue;

                            try
                            {
                                if (std::filesystem::is_directory(sourcePath))
                                    std::filesystem::copy(sourcePath, destinationPath, std::filesystem::copy_options::recursive);
                                else
                                    std::filesystem::copy_file(sourcePath, destinationPath);
                            }
                            catch (const std::filesystem::filesystem_error& e)
                            {
                                FG_ERROR("Failed to copy '{}' to '{}': {}", sourcePath.string(), destinationPath.string(), e.what());
                            }
                        }
                    }
                }

                if (!isCurrentRoot)
                {
                    if (ImGui::MenuItem("Delete", s_ShortCuts[ShortCut_Delete]))
                    {
                        m_TargetDeletePath = fullPath;

                        if (dontAskNextTime)
                            DeleteFileOrFolder();
                        else
                            showDeleteDialog = true;
                    }

                    if (ImGui::MenuItem("Rename", s_ShortCuts[ShortCut_Rename]))
                    {
                        m_RenamingNode = node;
                        strncpy_s(m_RenameBuffer, node->Name.c_str(), _TRUNCATE);
                    }
                }
            };

            if (node->isDirectory)
            {
                if (ImGui::BeginMenu("Add"))
                {
                    if (ImGui::MenuItem("New File", s_ShortCuts[ShortCut_AddFile]))
                        showNewItemModal = true;

                    if (ImGui::MenuItem("New Folder", s_ShortCuts[ShortCut_AddFolder])) {

                        std::filesystem::path newFolderPath = fullPath / "New Folder";

                        uint16_t counter = 1;
                        while (std::filesystem::exists(newFolderPath))
                        {
                            newFolderPath = fullPath / ("New Folder (" + std::to_string(counter) + ')');
                            counter++;
                        }

                        try
                        {
                            if (std::filesystem::create_directory(newFolderPath))
                                m_PathToRenameOnCreation = newFolderPath.lexically_normal();
                        }
                        catch (const std::filesystem::filesystem_error& e)
                        {
                            FG_ERROR("Failed to create folder '{}': {}", newFolderPath.string(), e.what());
                        }
                    }

                    ImGui::EndMenu();
                }

                ImGui::Separator();

                commonButtons();

                ImGui::Separator();

                if (ImGui::MenuItem("Copy File Path"))
                    fg::Utility::CopyToClipBoard(fullPath.generic_string());

#ifdef _WIN32
                if (ImGui::MenuItem("Open in File Explorer"))
                    fg::Utility::OpenInExplorer(fullPath);
#endif
            }
            else
            {
                auto handle = node->Id;

                if (!fg::AssetManager::IsAssetHandleValid(handle))
                {
                    if (ImGui::MenuItem("Import"))
                        m_AssetManager->RegisterAsset(std::filesystem::relative(fullPath, m_RootDirectory), handle);
                }
                else
                {
                    if (ImGui::MenuItem("Exclude"))
                        m_AssetManager->UnRegisterAsset(handle);
                }

                ImGui::Separator();

                commonButtons();

                ImGui::Separator();

                if (ImGui::MenuItem("Copy File Path"))
                    fg::Utility::CopyToClipBoard(fullPath.generic_string());

                if (ImGui::MenuItem("Open With..."))
                    fg::Utility::OpenInExplorer(fullPath);
            }

            ImGui::EndPopup();
        }
    }

    void ContentBrowser::RenderDeleteModal()
    {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(350.0f, 160.0f), ImGuiCond_Always);

        if (ImGui::BeginPopupModal("Delete?", &showDeleteDialog, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            if (m_TargetDeletePath.empty())
            {
                ImGui::EndPopup();
                return;
            }

            std::string info;
            if (std::filesystem::is_directory(m_TargetDeletePath))
                info = "'" + m_TargetDeletePath.filename().string() + "' and its item(s) will be deleted.\nThis operation cannot be undone!";
            else
                info = "'" + m_TargetDeletePath.filename().string() + "' will be deleted permanently!\nThis operation cannot be undone!";

            ImGui::TextWrapped(info.c_str());

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Checkbox("Don't ask me next time", &dontAskNextTime);
            ImGui::Spacing();

            float buttonWidth = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.15f, 0.15f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.10f, 0.10f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.60f, 0.05f, 0.05f, 1.0f));

            if (ImGui::Button("Delete", ImVec2(buttonWidth, 0.0f)))
            {
                DeleteFileOrFolder();
                m_TargetDeletePath.clear();
                ImGui::CloseCurrentPopup();
                showDeleteDialog = false;
            }

            ImGui::PopStyleColor(3);
            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0.0f)))
            {
                m_TargetDeletePath.clear();
                ImGui::CloseCurrentPopup();
                showDeleteDialog = false;
            }

            ImGui::EndPopup();
        }
    }

    void ContentBrowser::DeleteFileOrFolder()
    {
        std::filesystem::is_directory(m_TargetDeletePath) ? std::filesystem::remove_all(m_TargetDeletePath) : std::filesystem::remove(m_TargetDeletePath);
    }

    void ContentBrowser::RenderAddFileModal()
    {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(465.0f, 160.0f), ImGuiCond_Always);

        if (ImGui::BeginPopupModal("Add New Item", &showNewItemModal, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {

            if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            {
                showNewItemModal = false;
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                return;
            }

            static char nameBuffer[256] = "";

            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            ImGui::AlignTextToFramePadding();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30.0f);
            ImGui::Text("Name:");
            ImGui::SameLine();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 26.0f);
            ImGui::SetNextItemWidth(315.0f);
            ImGui::InputTextWithHint("##Name Input", "New File.txt", nameBuffer, sizeof(nameBuffer));

            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            std::filesystem::path dirPath = m_AssetTree.CurrentDirectory->Path;

            static char LocationBuffer[256] = "";

            ImGui::AlignTextToFramePadding();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30.0f);
            ImGui::Text("Location:");
            ImGui::SameLine();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
            ImGui::SetNextItemWidth(315.0f);
            ImGui::InputTextWithHint("##Location Input", dirPath.generic_string().c_str(), LocationBuffer, sizeof(LocationBuffer));

            ImGui::Dummy(ImVec2(0.0f, 15.0f));

            ImGui::SetCursorPosX(80.0f);
            if (ImGui::Button("Create", ImVec2(140.0f, 0.0f)))
            {
                std::string filename = (strlen(nameBuffer) > 0) ? nameBuffer : "New File.txt";
                std::filesystem::path fullpath = dirPath / filename;

                if (std::filesystem::exists(fullpath))
                {
                    FG_ERROR("File already exists: {}", fullpath.generic_string());
                }
                else
                {
                    std::ofstream fout(fullpath);
                    if (fout.is_open())
                        fout.close();
                    else
                        FG_ERROR("Failed to create file at: {}", fullpath.string());

                    nameBuffer[0] = '\0';
                    showNewItemModal = false;
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::SameLine(0.0f, 25.0f);
            if (ImGui::Button("Cancel", ImVec2(140.0f, 0.0f)))
            {
                showNewItemModal = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    fg::Scope<AssetNode> AssetTree::BuildNodeRecursively(const std::filesystem::path& fullPath, AssetNode* parent)
    {
        fg::Scope<AssetNode> node = fg::CreateScope<AssetNode>();
        node->Name = fullPath.filename().string();
        node->Path = fullPath;
        node->isDirectory = std::filesystem::is_directory(fullPath);
        node->Parent = parent;

        if (node->isDirectory)
            node->Id = fg::UUID();
        else
        {
            auto id = fg::Project::GetActive()->GetEditorAssetManager()->GetHandleFromRelativePath(std::filesystem::relative(node->Path, m_Root->Path));
            node->Id = (id != 0) ? id : fg::UUID();
        }

        m_AssetCache[node->Id] = node.get();

        if (node->isDirectory)
            for (const auto& entry : std::filesystem::directory_iterator(fullPath))
                node->Children.push_back(BuildNodeRecursively(entry.path(), node.get()));

        return node;
    }

    void AssetTree::BuildTreeFromDir(const std::filesystem::path& Rootpath)
    {
        m_AssetCache.clear();
        m_Root = fg::CreateScope<AssetNode>();
        m_Root->Id = fg::UUID();
        m_Root->Name = Rootpath.filename().string();
        m_Root->Path = Rootpath;
        m_Root->isDirectory = true;
        m_AssetCache[m_Root->Id] = m_Root.get();

        for (const auto& entry : std::filesystem::directory_iterator(Rootpath))
            m_Root->Children.push_back(BuildNodeRecursively(entry.path(), m_Root.get()));
    }

    void AssetTree::AddChild(fg::Scope<AssetNode>& child, const fg::UUID& ParentID)
    {
        AssetNode* parent = m_AssetCache[ParentID];
        parent->Children.push_back(std::move(child));
    }

    AssetNode* AssetTree::FindNode(const std::filesystem::path& Path)
    {
        if (!m_Root)
            return nullptr;

        const std::filesystem::path targetPath = Path.lexically_normal();
        std::function<AssetNode* (const std::filesystem::path& currentPath, AssetNode* current)> getNodebyPath;

        getNodebyPath = [&](const std::filesystem::path& currentPath, AssetNode* current) -> AssetNode*
            {
                if (current->Path.lexically_normal() == targetPath)
                    return current;

                for (const auto& child : current->Children)
                {
                    if (child->isDirectory)
                    {
                        AssetNode* found = getNodebyPath(targetPath, child.get());
                        if (found)
                            return found;
                    }
                    else
                        if (child->Path.lexically_normal() == targetPath)
                            return child.get();
                }
                return nullptr;
            };
        return getNodebyPath(Path, m_Root.get());
    }

    bool AssetTree::OnFileChange(const fg::Event::FileChange& e)
    {
        switch (e.Action)
        {
        case fg::Event::FileChange::Add:
        {
            const std::filesystem::path path = (std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName)).lexically_normal();

            // Recursive copy operations commonly emit an Add event for both the
            // folder and every item inside it. The folder event already builds its
            // descendants, so ignore the follow-up events rather than duplicating nodes.
            if (FindNode(path))
                return true;

            AssetNode* parentNode = FindNode(path.parent_path());

            if (!parentNode || !std::filesystem::exists(path))
                return false;

            parentNode->Children.push_back(BuildNodeRecursively(path, parentNode));
            return true;
        }
        case fg::Event::FileChange::Delete:
        {
            const std::filesystem::path path = (std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName)).lexically_normal();

            AssetNode* nodeToDelete = FindNode(path);
            if (!nodeToDelete)
                return false;

            AssetNode* parentNode = nodeToDelete->Parent;
            if (!parentNode)
                return false;

            std::function<void(AssetNode*)> unregisterFromCache = [&](AssetNode* node) {
                for (const auto& child : node->Children)
                {
                    if (child->isDirectory)
                        unregisterFromCache(child.get());
                    else
                        fg::Project::GetActive()->GetEditorAssetManager()->UnRegisterAsset(child->Id);
                    m_AssetCache.erase(child->Id);
                }
                };

            if (nodeToDelete->isDirectory)
                unregisterFromCache(nodeToDelete);
            else
                fg::Project::GetActive()->GetEditorAssetManager()->UnRegisterAsset(nodeToDelete->Id);

            m_AssetCache.erase(nodeToDelete->Id);

            auto& parentChildren = parentNode->Children;
            auto it = std::find_if(parentChildren.begin(), parentChildren.end(),
                [nodeToDelete](const fg::Scope<AssetNode>& child) {
                    return child.get() == nodeToDelete;
                });

            if (it != parentChildren.end())
            {
                parentChildren.erase(it);

                auto belongsToDeletedSubtree = [nodeToDelete](AssetNode* node) {
                    for (AssetNode* current = node; current; current = current->Parent)
                        if (current == nodeToDelete)
                            return true;
                    return false;
                };

                if (belongsToDeletedSubtree(CurrentDirectory))
                    CurrentDirectory = parentNode;
                if (belongsToDeletedSubtree(SelectedNode))
                    SelectedNode = nullptr;
                return true;
            }
            return false;
        }
        case fg::Event::FileChange::Modified:
        {
            return true;
        }
        case fg::Event::FileChange::Moved:
        {
            std::filesystem::path oldPath = (std::filesystem::path(e.Directory) / std::filesystem::path(e.OldFilename)).lexically_normal();
            std::filesystem::path newPath = (std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName)).lexically_normal();

            AssetNode* currentNode = FindNode(oldPath);
            if (!currentNode)
                return false;

            currentNode->Name = newPath.filename().string();
            currentNode->Path = newPath;

            if (!currentNode->isDirectory && fg::AssetManager::IsAssetHandleValid(currentNode->Id))
            {
                const auto& oldMetaData = fg::AssetManager::GetAssetMetaData(currentNode->Id);
                fg::AssetMetaData newMetaData = { oldMetaData.Type, std::filesystem::relative(newPath, m_Root->Path) };
                fg::Project::GetActive()->GetEditorAssetManager()->ModifyAsset(currentNode->Id, newMetaData);
            }

            std::filesystem::path oldParentPath = oldPath.parent_path();
            std::filesystem::path newParentPath = newPath.parent_path();

            if (oldParentPath != newParentPath)
            {
                AssetNode* newParentNode = FindNode(newParentPath);
                AssetNode* oldParentNode = currentNode->Parent;

                if (newParentNode && oldParentNode)
                {
                    auto& oldChildren = oldParentNode->Children;
                    auto it = std::find_if(oldChildren.begin(), oldChildren.end(),
                        [currentNode](const fg::Scope<AssetNode>& child) {
                            return child.get() == currentNode;
                        });

                    if (it != oldChildren.end())
                    {
                        fg::Scope<AssetNode> movedNodeOwnership = std::move(*it);
                        oldChildren.erase(it);

                        currentNode->Parent = newParentNode;
                        newParentNode->Children.push_back(std::move(movedNodeOwnership));
                    }
                }
            }

            if (currentNode->isDirectory)
            {
                std::function<void(AssetNode*)> updateChildPaths = [&](AssetNode* parentNode)
                    {
                        for (auto& child : parentNode->Children)
                        {
                            child->Path = parentNode->Path / child->Name;

                            if (!child->isDirectory && fg::AssetManager::IsAssetHandleValid(child->Id))
                            {
                                const auto& oldMeta = fg::AssetManager::GetAssetMetaData(child->Id);
                                fg::AssetMetaData newMeta = { oldMeta.Type, std::filesystem::relative(child->Path, m_Root->Path) };
                                fg::Project::GetActive()->GetEditorAssetManager()->ModifyAsset(child->Id, newMeta);
                            }

                            if (child->isDirectory)
                                updateChildPaths(child.get());
                        }
                    };

                updateChildPaths(currentNode);
            }

            return true;
        }
        }

        return false;
    }
}
