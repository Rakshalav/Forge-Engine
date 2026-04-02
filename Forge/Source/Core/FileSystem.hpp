//#pragma once
//
//#include <filesystem>
//
//#ifdef _WIN32
//    #ifndef WIN32_LEAN_AND_MEAN
//        #define WIN32_LEAN_AND_MEAN 
//    #endif
//    #include <Windows.h>
//#endif
//
//#include "Scene/Mesh.hpp"
//
//namespace fs = std::filesystem;
//
//namespace fg
//{
//    class FileSystem
//    {
//    public:
//        static inline fs::path GetExecutablePath()
//        {
//            #ifdef _WIN32
//                wchar_t path[MAX_PATH];
//                if (GetModuleFileNameW(NULL, path, MAX_PATH) == 0)
//                    return ""; 
//                return fs::path(path).parent_path();
//            #else
//                return fs::current_path();
//            #endif
//        }
//
//        static inline fs::path GetProjectRoot()
//        {
//            fs::path current = GetExecutablePath();
//
//            for (uint8_t i = 0; i < 4; i++)
//                if (current.has_parent_path())
//                    current = current.parent_path();
//            return current;
//        }
//
//        static Ref<Mesh> LoadModel(fs::path path);
//    };
//}
//
//#define FG_EXECUTABLE_DIR fg::FileSystem::GetExecutablePath().string()
//#define FG_ROOT_DIR(relativePath) (fg::FileSystem::GetProjectRoot() / relativePath).string()