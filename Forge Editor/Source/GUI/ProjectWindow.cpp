#include "ProjectWindow.hpp"
#include <functional>

namespace Editor
{
    ProjectWindow::ProjectWindow()
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

            m_AssetTree.CurrentNode = m_AssetTree.GetRoot();
        }
    }

    void ProjectWindow::OnEvent(fg::Event& event)
    {
        fg::Event::Print<fg::EventType::FileChange>(event);

        fg::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<fg::Event::FileChange>([this](fg::Event::FileChange& e) { return m_AssetTree.OnFileChange(e); });
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
                    RenderTreeView(m_AssetTree.GetRoot());
                    RenderContextMenu(m_AssetTree.GetRoot(), true);
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

    void ProjectWindow::RenderTreeView(AssetNode* node)
    {
        const auto& children = node->Children;
        
        for (const auto& child : children)
        {
            if (child->isDirectory)
            {
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

                if (m_AssetTree.CurrentNode == child.get())
                    flags |= ImGuiTreeNodeFlags_Selected;

                bool isNodeOpen = ImGui::TreeNodeEx((void*)(uintptr_t)child->Id, flags, "%s", child->Name.c_str());

                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                    m_AssetTree.CurrentNode = child.get();

                RenderContextMenu(child.get());

                if (isNodeOpen)
                {
                    RenderTreeView(child.get());
                    ImGui::TreePop();
                }
            }
            else
            {
                auto handle = child->Id;

                bool isImported = (handle != 0);
                if (!isImported)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

                bool isSelected = (m_AssetTree.SelectedNode == child.get());

                ImGui::PushID((void*)(uintptr_t)child->Id);

                if (ImGui::Selectable(child->Name.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
                    m_AssetTree.SelectedNode = child.get();

                RenderContextMenu(child.get());

                ImGui::PopID();

                if (!isImported)
                    ImGui::PopStyleColor();
            }
        }
    }

    void ProjectWindow::RenderContextMenu(AssetNode* node, bool isWindowContext)
    {
        bool popupOpen = isWindowContext ? ImGui::BeginPopupContextWindow("RootContextMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup) : ImGui::BeginPopupContextItem();

        if (popupOpen)
        {
            std::filesystem::path fullPath = m_RootDirectory / node->RelativePath;

            if (node->isDirectory)
            {
                m_AssetTree.CurrentNode = node;

                if (ImGui::MenuItem("New Folder")) {}

                if (node != m_AssetTree.GetRoot())
                    if (ImGui::MenuItem("Delete Folder"))
                        std::filesystem::remove_all(fullPath);

                if (ImGui::MenuItem("Copy File Path"))
                    fg::Utility::CopyToClipBoard(fullPath.generic_string());

#ifdef _WIN32
                if (ImGui::MenuItem("Open in File Explorer"))
                    fg::Utility::OpenInExplorer(fullPath);
#endif
            }
            else
            {
                m_AssetTree.SelectedNode = node;

                auto handle = node->Id;

                if (!fg::AssetManager::IsAssetHandleValid(handle))
                {
                    if (ImGui::MenuItem("Import"))
                        m_AssetManager->RegisterAsset(node->RelativePath, handle);
                }
                else
                {
                    if (ImGui::MenuItem("Exclude"))
                        m_AssetManager->UnRegisterAsset(handle);
                }

                if (ImGui::MenuItem("Delete"))
                    std::filesystem::remove(fullPath);
                if (ImGui::MenuItem("Copy File Path"))
                    fg::Utility::CopyToClipBoard(fullPath.generic_string());
            }

            ImGui::EndPopup();
        }
    }

    fg::Scope<AssetNode> AssetTree::BuildNodeRecursively(const std::filesystem::path& fullPath, AssetNode* parent)
    {
        fg::Scope<AssetNode> node = fg::CreateScope<AssetNode>();
        node->Name = fullPath.filename().string();
        node->RelativePath = std::filesystem::relative(fullPath, fg::Project::GetAssetDirectory());
        node->isDirectory = std::filesystem::is_directory(fullPath);
        node->Parent = parent;

        if (node->isDirectory)
            node->Id = fg::UUID();
        else
        {
            auto id = fg::Project::GetActive()->GetEditorAssetManager()->GetHandleFromRelativePath(node->RelativePath);
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
        m_Root->RelativePath = "";
        m_Root->isDirectory = true;
        m_AssetCache[m_Root->Id] = m_Root.get();

        for (const auto& entry : std::filesystem::directory_iterator(Rootpath))
            m_Root->Children.push_back(BuildNodeRecursively(entry.path(), m_Root.get()));
    }

    AssetNode* AssetTree::FindNode(const std::filesystem::path& relativePath)
    {
        std::function<AssetNode*(const std::filesystem::path& currentPath, AssetNode* current)> getNodebyPath;

        getNodebyPath = [&](const std::filesystem::path& currentPath, AssetNode* current) -> AssetNode*
        {
            if (current->RelativePath == relativePath)
                return current;

            for (const auto& child : current->Children)
            {
                if (child->isDirectory)
                {
                    AssetNode* found = getNodebyPath(relativePath, child.get());
                    if (found) 
                        return found; 
                }
                else
                    if (child->RelativePath == relativePath)
                        return child.get();
            }
                return nullptr;
        };
        return getNodebyPath(relativePath, m_Root.get());
    }

    bool AssetTree::OnFileChange(const fg::Event::FileChange& e)
    {
        enum Action { Add = 1, Delete = 2, Modified = 3, Moved = 4 };
        switch (e.Action)
        {
        case Add:
        {
            std::filesystem::path fullPath = std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName);
            std::filesystem::path relativePath = std::filesystem::relative(fullPath, fg::Project::GetAssetDirectory());

            AssetNode* parentNode = (relativePath == relativePath.filename()) ? m_Root.get() : FindNode(relativePath.parent_path());

            if (!parentNode)
                return false;

            parentNode->Children.push_back(BuildNodeRecursively(fullPath, parentNode));
            return true;
        }
        case Delete:
        {
            std::filesystem::path fullPath = std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName);
            std::filesystem::path relativePath = std::filesystem::relative(fullPath, fg::Project::GetAssetDirectory());

            AssetNode* nodeToDelete = FindNode(relativePath);
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

                if (SelectedNode == nodeToDelete) 
                    SelectedNode = nullptr;
                if (CurrentNode == nodeToDelete) 
                    CurrentNode = parentNode; 
                return true;
            }
            return false;
        }
        case Modified:
        {

        }
        case Moved:
        {
            std::filesystem::path fullPath = std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName);
            std::filesystem::path relativePath = std::filesystem::relative(fullPath, fg::Project::GetAssetDirectory());

            std::filesystem::path oldFullPath = std::filesystem::path(e.Directory) / std::filesystem::path(e.OldFilename);
            std::filesystem::path oldRelativePath = std::filesystem::relative(oldFullPath, fg::Project::GetAssetDirectory());

            AssetNode* currentNode = FindNode(oldRelativePath);

            if (currentNode) 
            {
                currentNode->Name = e.FileName;
                currentNode->RelativePath = relativePath;

                if (fg::AssetManager::IsAssetHandleValid(currentNode->Id)) {

                    const auto& oldMetaData = fg::AssetManager::GetAssetMetaData(currentNode->Id);
                    fg::AssetMetaData newMetaData = { oldMetaData.Type, relativePath };

                    fg::Project::GetActive()->GetEditorAssetManager()->ModifyAsset(currentNode->Id, newMetaData);
                }

                return true;
            }
            return false;
        }
        }
    }
}