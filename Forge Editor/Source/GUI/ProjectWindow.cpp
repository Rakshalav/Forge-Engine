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

    void ProjectWindow::RenderContextMenu(AssetNode* node)
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (node->isDirectory)
            {
                m_AssetTree.CurrentNode = node;

                ImGui::TextDisabled("Folder Options");
                ImGui::Separator();

                if (ImGui::MenuItem("New Folder")) {  }
                if (ImGui::MenuItem("Delete Folder"))
                    std::filesystem::remove_all(m_RootDirectory / node->RelativePath);
            }
            else
            {
                m_AssetTree.SelectedNode = node;

                ImGui::TextDisabled("Asset Options");
                ImGui::Separator();

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

                std::filesystem::path fullPath = m_RootDirectory / node->RelativePath;
                if (ImGui::MenuItem("Delete"))
                    std::filesystem::remove(fullPath);
                if (ImGui::MenuItem("Copy File Path"))
                    fg::Utility::CopyToClipBoard(fullPath.generic_string());
            }

            ImGui::EndPopup();
        }
    }

    void AssetTree::BuildTreeFromDir(const std::filesystem::path& Rootpath)
    {
        m_Root = fg::CreateScope<AssetNode>();
        m_Root->Id = fg::UUID();
        m_Root->Name = Rootpath.filename().string();
        m_Root->RelativePath = Rootpath; 
        m_Root->isDirectory = true;

        m_AssetCache[m_Root->Id] = m_Root.get();

        std::function<void(const std::filesystem::path& currentDir, AssetNode* parentNode)> iteratethroughDir;

        iteratethroughDir = [&](const std::filesystem::path& currentDir, AssetNode* parentNode) -> void 
        {
            for (const auto& entry : std::filesystem::directory_iterator(currentDir))
            {
                const auto& path = entry.path();

                fg::Scope<AssetNode> childNode = fg::CreateScope<AssetNode>();
                childNode->Name = path.filename().string();

                childNode->RelativePath = std::filesystem::relative(path, Rootpath);
                childNode->isDirectory = entry.is_directory();
                childNode->Parent = parentNode;

                auto id = fg::Project::GetActive()->GetEditorAssetManager()->GetHandleFromRelativePath(childNode.get()->RelativePath);
                id != 0 ? childNode->Id = id : childNode->Id = fg::UUID();

                AssetNode* childPtr = childNode.get();
                m_AssetCache[childNode->Id] = childPtr;

                parentNode->Children.push_back(std::move(childNode));

                if (childPtr->isDirectory)
                    iteratethroughDir(path, childPtr);
            }
        };
        iteratethroughDir(Rootpath, m_Root.get());
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
            bool isDirectory = std::filesystem::is_directory(fullPath);
            std::filesystem::path relativePath = std::filesystem::relative(fullPath, m_Root->RelativePath);

            auto newNode = fg::CreateScope<AssetNode>();

            newNode->Name = e.FileName;
            newNode->RelativePath = relativePath;

            if (isDirectory)
                newNode->Id = fg::UUID();
            else
            {
                auto id = fg::Project::GetActive()->GetEditorAssetManager()->GetHandleFromRelativePath(newNode->RelativePath);
                newNode->Id = (id != 0) ? id : fg::UUID();
            }

            AssetNode* parentNode = nullptr;
            if (relativePath == relativePath.filename())
                parentNode = m_Root.get();
            else
                parentNode = FindNode(relativePath.parent_path());

            if (parentNode)
            {
                newNode->Parent = parentNode;
                m_AssetCache[newNode->Id] = newNode.get();
                parentNode->Children.push_back(std::move(newNode));
                return true;
            }
            else
                return false;
        }
        case Delete:
        {
            std::filesystem::path fullPath = std::filesystem::path(e.Directory) / std::filesystem::path(e.FileName);
            std::filesystem::path relativePath = std::filesystem::relative(fullPath, m_Root->RelativePath);

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
            std::filesystem::path relativePath = std::filesystem::relative(fullPath, m_Root->RelativePath);

            std::filesystem::path oldFullPath = std::filesystem::path(e.Directory) / std::filesystem::path(e.OldFilename);
            std::filesystem::path oldRelativePath = std::filesystem::relative(oldFullPath, m_Root->RelativePath);

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
