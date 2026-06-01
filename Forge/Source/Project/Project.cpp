#include "Project.hpp"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "Debug/Log.hpp"

namespace fg
{
    namespace ProjectSerializer
    {
        static bool Serialize(Ref<Project> project, const std::filesystem::path& filepath)
        {
            const auto& config = project->GetConfig();

            YAML::Emitter out;
            {
                out << YAML::BeginMap;
                out << YAML::Key << "Project" << YAML::Value;
                {
                    out << YAML::BeginMap;
                    out << YAML::Key << "Name" << YAML::Value << config.Name;
                    out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
                    out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
                    out << YAML::Key << "AssetRegistry" << YAML::Value << config.AssetRegistry.string();
                    out << YAML::EndMap;
                }
                out << YAML::EndMap;
            }
            std::ofstream fout(filepath);
            if (!fout.is_open())
                return false;
            fout << out.c_str();
            return true;
        }

        static bool Deserialize(Ref<Project> project, const std::filesystem::path& filepath)
        {
            auto& config = project->GetConfig();

            YAML::Node data;
            try
            {
                data = YAML::LoadFile(filepath.string());
            }
            catch (YAML::ParserException e)
            {
                FG_CORE_ERROR("Failed to load project file '{}'\n {}", filepath.string(), e.what());
                return false;
            }

            auto projectNode = data["Project"];
            if (!projectNode)
                return false;

            config.Name = projectNode["Name"].as<std::string>();
            config.StartScene = projectNode["StartScene"].as<std::string>();
            config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
            config.AssetRegistry = projectNode["AssetRegistry"].as<std::string>();
            return true;
        }
    }

    Ref<Project> Project::New()
    {
        s_ActiveProject = CreateRef<Project>();
        return s_ActiveProject;
    }

    Ref<Project> Project::Load(const std::filesystem::path& path)
    {
        Ref<Project> project = CreateRef<Project>();

        if (ProjectSerializer::Deserialize(project, path))
        {
            project->m_ProjectDirectory = path.parent_path();
            project->m_ProjectDirectory = path.parent_path();
            s_ActiveProject = project;
            s_ActiveProject->m_AssetManager = CreateRef<AssetManagerEditor>();

            if (!StaticRefCast<AssetManagerEditor>(s_ActiveProject->m_AssetManager)->DeserializeAssetRegistry())
                FG_CORE_ERROR("Failed to deserialize asset registry!");

            return s_ActiveProject;
        }   

        return nullptr;
    }

    bool Project::Create(const std::string& name, const std::filesystem::path& projectDirectory)
    {
        New();
        auto& config = s_ActiveProject->GetConfig();
        config.Name = name;

        s_ActiveProject->m_ProjectDirectory = projectDirectory;

        if (!std::filesystem::exists(projectDirectory))
        {
            std::filesystem::create_directories(projectDirectory / config.AssetDirectory);
            std::filesystem::create_directories(projectDirectory / config.AssetDirectory / "Scenes");
        }

        std::filesystem::path projectFilePath = projectDirectory / (name + ".fgproj");

        if (!ProjectSerializer::Serialize(s_ActiveProject, projectFilePath))
        {
            return false;
        }

        std::filesystem::path registryPath = projectDirectory / config.AssetRegistry;
        if (!std::filesystem::exists(registryPath))
        {
            YAML::Emitter out;
            out << YAML::BeginMap;
            out << YAML::Block; 
            out << YAML::Key << "Assets" << YAML::Value << YAML::Null; 
            out << YAML::EndMap;

            std::ofstream fout(registryPath);
            fout << out.c_str();
            fout.close();
        }

        return true;
    }

    bool Project::SaveActive(const std::filesystem::path& path)
    {
        if (ProjectSerializer::Serialize(s_ActiveProject, path))
        {
            s_ActiveProject->m_ProjectDirectory = path.parent_path();

            if (!StaticRefCast<AssetManagerEditor>(s_ActiveProject->m_AssetManager)->SerializeAssetRegistry())
            {
                FG_CORE_ERROR("Failed to save project!");
                return false;
            }
            return true;
        }

        return false;
    }
}