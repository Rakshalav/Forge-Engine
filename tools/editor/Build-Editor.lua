project "Editor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++23"
   staticruntime "on"

   targetname "Forge Editor"

   flags { "multiprocessorcompile" }

   defines { 
      "YAML_CPP_STATIC_DEFINE", 
      "_CRT_SECURE_NO_WARNINGS" 
   }
   linkoptions { "/ignore:4099" }

   targetdir ("../../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../../bin/Intermediates/" .. OutputDir .. "/%{prj.name}")

   files { 
      "include/**.h",
      "include/**.hpp",
      "src/**.h",
      "src/**.hpp",
      "src/**.cpp"
   }

   includedirs {
      "include",
      "src",
      "../../engine/include",
      "../../engine/src",
      "../../engine/platform",
      "../../engine/dep",
      "../../engine/dep/imgui"
   }

   -- System & Engine Dependencies
   links { 
      "Engine",
      "opengl32", 
      "user32", 
      "gdi32", 
      "shell32" 
   }

   filter "configurations:Debug"
      libdirs { 
         vcpkg_installed .. "/debug/lib",
         "../../bin/" .. OutputDir .. "/Engine"
      }
      links { 
         "assimp-vc143-mtd",
         "efsw",
         "fmt-c",
         "fmtd",
         "glad",
         "glfw3",
         "glm",
         "kubazip",
         "minizipsd",
         "poly2tri",
         "polyclipping",
         "pugixml",
         "spdlogd",
         "yaml-cppd",
         "zsd"
      }

   filter { "configurations:Release", "configurations:Dist" }
      libdirs { 
         vcpkg_installed .. "/lib",
         "../../bin/" .. OutputDir .. "/Engine"
      }
      links { 
         "assimp-vc143-mt",
         "efsw",
         "fmt-c",
         "fmt",
         "glad",
         "glfw3",
         "glm",
         "kubazip",
         "minizips",
         "poly2tri",
         "polyclipping",
         "pugixml",
         "spdlog",
         "yaml-cpp",
         "zs"
      }

   filter {}

   filter "system:windows"
      systemversion "latest"
      defines { "WINDOWS" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      defines { "DIST" }
      kind "WindowedApp"
      runtime "Release"
      optimize "On"
      symbols "Off"