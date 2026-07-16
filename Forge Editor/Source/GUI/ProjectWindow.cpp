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
                    /*RenderTreeView(m_AssetTree.GetRoot());
                    RenderContextMenu(m_AssetTree.GetRoot(), true);*/
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

    void ContentBrowser::RenderTreeView(AssetNode* node)
    {
        //const auto& children = node->Children;

        //for (const auto& child : children)
        //{
            /*if (!m_PathToRenameOnCreation.empty() && child->Path == m_PathToRenameOnCreation)
            {
                m_RenamingNode = child.get();
                strncpy(m_RenameBuffer, child->Name.c_str(), sizeof(m_RenameBuffer));
                m_PathToRenameOnCreation.clear();
            }

            if (m_RenamingNode == child.get())
            {
                ImGui::SetKeyboardFocusHere();
                ImGui::AlignTextToFramePadding();

                if (ImGui::InputText("##RenameBox", m_RenameBuffer, IM_ARRAYSIZE(m_RenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (strlen(m_RenameBuffer) > 0 && m_RenameBuffer != child->Name)
                    {
                        std::filesystem::path oldPath = child->Path;
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

                if (ImGui::IsItemDeactivated() || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
                    m_RenamingNode = nullptr;
            }*/
        //    else
        //    {
        //        if (child->isDirectory)
        //        {
        //            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DrawLinesToNodes;

        //            if (m_AssetTree.CurrentDirectory == child.get())
        //                flags |= ImGuiTreeNodeFlags_Selected;


        //            bool isNodeOpen = ImGui::TreeNodeEx((void*)(uintptr_t)child->Id, flags, "%s", child->Name.c_str());

        //            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        //            {
        //                m_AssetTree.CurrentDirectory = child.get();
        //                m_AssetTree.SelectedNode = child.get();
        //            }

        //            if ((m_AssetTree.SelectedNode == child.get()) && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImGui::IsKeyPressed(ImGuiKey_F2))
        //            {
        //                m_RenamingNode = child.get();
        //                strncpy(m_RenameBuffer, child->Name.c_str(), sizeof(m_RenameBuffer));
        //            }

        //            if ((m_AssetTree.SelectedNode == child.get()) && ImGui::IsKeyPressed(ImGuiKey_Delete))
        //            {
        //                m_TargetDeletePath = m_AssetTree.SelectedNode->Path;

        //                if (dontAskNextTime)
        //                    DeleteFileOrFolder();
        //                else
        //                    showDeleteDialog = true;
        //            }

        //            RenderContextMenu(child.get());

        //            if (isNodeOpen)
        //            {
        //                RenderTreeView(child.get());
        //                ImGui::TreePop();
        //            }
        //        }
        //        else
        //        {
        //            auto handle = child->Id;

        //            bool isImported = (handle != 0);
        //            if (!isImported)
        //                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

        //            bool isSelected = (m_AssetTree.SelectedNode == child.get());

        //            ImGui::PushID((void*)(uintptr_t)child->Id);

        //            if (ImGui::Selectable(child->Name.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
        //                m_AssetTree.SelectedNode = child.get();

        //            if (isSelected && ImGui::IsKeyPressed(ImGuiKey_Delete))
        //            {
        //                m_TargetDeletePath = m_AssetTree.SelectedNode->Path;

        //                if (dontAskNextTime)
        //                    DeleteFileOrFolder();
        //                else
        //                    showDeleteDialog = true;
        //            }

        //            RenderContextMenu(child.get());

        //            ImGui::PopID();

        //            if (!isImported)
        //                ImGui::PopStyleColor();
        //        }
        //    }
        //}
    }

    void ContentBrowser::HierarchyPanel()
    {
        std::function<void(const AssetNode*)> drawTree;
        drawTree = [&](const AssetNode* currentDir)
        {
            for (const auto& child : currentDir->Children)
            {
                if (!m_PathToRenameOnCreation.empty() && child->Path == m_PathToRenameOnCreation)
                {
                    m_RenamingNode = child.get();
                    strncpy(m_RenameBuffer, child->Name.c_str(), sizeof(m_RenameBuffer));
                    m_PathToRenameOnCreation.clear();
                }

                if (m_RenamingNode == child.get())
                    RenameFileOrFolder();
                else
                {
                    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
                    if (!child->isDirectory)
                        flags |= ImGuiTreeNodeFlags_Leaf;
                    else
                        flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DrawLinesNone;

                    // isOpen = true ? it's a directory : it's a file
                    bool isOpen = ImGui::TreeNodeEx((void*)(uintptr_t)child->Id, flags, "%s", child->Name.c_str());

                    RenderContextMenu(child.get());

                    if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
                        m_AssetTree.CurrentDirectory = child.get();

                    if (isOpen)
                    {
                        drawTree(child.get());
                        ImGui::TreePop();
                    }
                }
            }
        };

        ImGuiTreeNodeFlags rootNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesNone | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        bool rootOpen = ImGui::TreeNodeEx((void*)(uintptr_t)m_AssetTree.GetRoot()->Id, rootNodeFlags, "%s", m_AssetTree.GetRoot()->Name.c_str());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
            m_AssetTree.CurrentDirectory = m_AssetTree.GetRoot();
      
        if (rootOpen)
        {
            RenderContextMenu(m_AssetTree.GetRoot());
            drawTree(m_AssetTree.GetRoot());
            ImGui::TreePop();
        }
    }

    void ContentBrowser::RenameFileOrFolder()
    {
        ImGui::SetKeyboardFocusHere();
        //ImGui::AlignTextToFramePadding();

        float textWidth = ImGui::CalcTextSize(m_RenameBuffer).x;
        float extraWidthPadding = 10.0f;
        ImGui::SetNextItemWidth(textWidth + extraWidthPadding);
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, style.FramePadding.y));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

        if (ImGui::InputText("##RenameBox", m_RenameBuffer, IM_ARRAYSIZE(m_RenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
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

        if (ImGui::IsItemDeactivated() || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
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
                        if (!m_AssetTree.SelectedNodes.empty())
                            m_AssetTree.SelectedNodes.clear();
                        m_AssetTree.SelectedNodes.push_back(node);
                        FG_INFO("{} copied!", node->Name);
                    }
                }

                if (isCurrentDir)
                {
                    if (ImGui::MenuItem("Paste", s_ShortCuts[ShortCut_Paste], false, !m_AssetTree.SelectedNodes.empty()))
                    {
                        for (const auto& node : m_AssetTree.SelectedNodes)
                        {
                            auto dirPath = fullPath / node->Name;
                            if (!std::filesystem::exists(dirPath))
                            {
                                if (node->isDirectory)
                                {
                                    if (std::filesystem::create_directory(dirPath))
                                        std::filesystem::copy(fullPath, dirPath, std::filesystem::copy_options::recursive);
                                }
                                else
                                    std::filesystem::copy_file(fullPath, dirPath);
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
                        strncpy(m_RenameBuffer, node->Name.c_str(), sizeof(m_RenameBuffer));
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

                        std::filesystem::create_directory(newFolderPath);

                        m_PathToRenameOnCreation = std::filesystem::relative(newFolderPath, m_RootDirectory);
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
        std::function<AssetNode* (const std::filesystem::path& currentPath, AssetNode* current)> getNodebyPath;

        getNodebyPath = [&](const std::filesystem::path& currentPath, AssetNode* current) -> AssetNode*
            {
                if (current->Path == Path)
                    return current;

                for (const auto& child : current->Children)
                {
                    if (child->isDirectory)
                    {
                        AssetNode* found = getNodebyPath(Path, child.get());
                        if (found)
                            return found;
                    }
                    else
                        if (child->Path == Path)
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
            std::filesystem::path path = std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName);

            AssetNode* parentNode = FindNode(path.parent_path());

            if (!parentNode)
                return false;

            parentNode->Children.push_back(BuildNodeRecursively(path, parentNode));
            return true;
        }
        case fg::Event::FileChange::Delete:
        {
            std::filesystem::path path = std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName);

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

                if (CurrentDirectory == nodeToDelete)
                    CurrentDirectory = parentNode;
                return true;
            }
            return false;
        }
        case fg::Event::FileChange::Modified:
        {

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
    }
}