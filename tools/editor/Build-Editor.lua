project "Editor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++23"
   staticruntime "off"

   -- File will be built as "Forge Editor.exe"
   targetname "Forge Editor"

   flags { "multiprocessorcompile" }

   defines { "YAML_CPP_STATIC_DEFINE", "_CRT_SECURE_NO_WARNINGS" }
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

   -- vcpkg Libraries (Debug)
   filter "configurations:Debug"
      libdirs { 
         vcpkg_installed .. "/debug/lib",
         "../../bin/" .. OutputDir .. "/Engine"
      }
      links { 
         "glfw3dll", 
         "glad", 
         "spdlogd", 
         "fmtd",
         "efsw", 
         "yaml-cppd", 
         "assimp-vc143-mtd" 
      }
      postbuildcommands {
         "{COPY} " .. vcpkg_installed .. "/debug/bin/assimp-vc143-mtd.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/efsw.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/fmtd.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/glfw3.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/kubazip.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/minizipd.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/poly2tri.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/pugixml.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/spdlogd.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/yaml-cppd.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/debug/bin/zd.dll \"%{cfg.targetdir}\""
      }

   -- vcpkg Libraries (Release & Dist)
   filter { "configurations:Release", "configurations:Dist" }
      libdirs { 
         vcpkg_installed .. "/lib",
         "../../bin/" .. OutputDir .. "/Engine"
      }
      links { 
         "glfw3dll", 
         "glad", 
         "spdlog", 
         "fmt",
         "efsw",
         "yaml-cpp", 
         "assimp-vc143-mt" 
      }
      postbuildcommands {
         "{COPY} " .. vcpkg_installed .. "/bin/assimp-vc143-mt.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/efsw.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/fmt.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/glfw3.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/kubazip.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/minizip.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/poly2tri.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/pugixml.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/spdlog.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/yaml-cpp.dll \"%{cfg.targetdir}\"",
         "{COPY} " .. vcpkg_installed .. "/bin/z.dll \"%{cfg.targetdir}\""
      }

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