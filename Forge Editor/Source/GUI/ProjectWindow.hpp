#pragma once
#include <efsw/efsw.hpp>
#include <Forge.hpp>
#include <Core/Filelistener.hpp>

namespace Editor
{
    struct AssetNode
    {
        fg::UUID Id;
        std::string Name;
        std::filesystem::path Path;
        bool isDirectory;
        AssetNode* Parent = nullptr;
        std::vector<fg::Scope<AssetNode>> Children;
    };

    class AssetTree
    {
    public:
        void BuildTreeFromDir(const std::filesystem::path& Rootpath);
        AssetNode* GetRoot() const { return m_Root.get(); }
        bool OnFileChange(const fg::Event::FileChange& e);
        AssetNode* operator[](const fg::UUID& id) {
            if (!m_AssetCache.contains(id))
                return nullptr;
            else
                return m_AssetCache[id];
        }
        AssetNode* operator[](const std::filesystem::path& Path) {
            return FindNode(Path);
        }
        void AddChild(fg::Scope<AssetNode>& child, const fg::UUID& ParentID);
    public:
        std::vector<AssetNode*> SelectedNodes;
        AssetNode* CurrentDirectory = nullptr;
    private:
        AssetNode* FindNode(const std::filesystem::path& Path);
        fg::Scope<AssetNode> BuildNodeRecursively(const std::filesystem::path& fullPath, AssetNode* parent);
    private:
        fg::Scope<AssetNode> m_Root;
        std::unordered_map<fg::UUID, AssetNode*> m_AssetCache;
    };

    class ContentBrowser
    {
    public:
        ContentBrowser();
        void OnEvent(fg::Event& event);
        void OnUpdate(float dt);
        void OnRender();
    private:
        void RenderTreeView(AssetNode* node);
        void RenderGridView(AssetNode* currentDirectory);
        void RenderContextMenu(AssetNode* node);
        void RenderDeleteModal();
        void DeleteFileOrFolder();
        void RenderAddFileModal();
        void HierarchyPanel();
        void RenameFileOrFolder();
    private:
        std::filesystem::path m_RootDirectory;
        fg::Ref<fg::AssetManagerEditor> m_AssetManager;
        efsw::FileWatcher* m_FileWatcher = nullptr;
        fg::UpdateListener* m_UpdateListener = nullptr;
        AssetTree m_AssetTree;
        std::filesystem::path m_TargetDeletePath;
        AssetNode* m_RenamingNode = nullptr;
        char m_RenameBuffer[256] = "";
        std::filesystem::path m_PathToRenameOnCreation;
    };
}