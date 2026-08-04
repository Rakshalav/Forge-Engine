project "Engine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++23"
   staticruntime "off"

   -- File will be built as Forge.lib
   targetname "Forge"

   flags { "multiprocessorcompile" }

   defines { "YAML_CPP_STATIC_DEFINE", "_CRT_SECURE_NO_WARNINGS" }
   linkoptions { "/ignore:4099" }

   targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../bin/Intermediates/" .. OutputDir .. "/%{prj.name}")

   files { 
      "include/**.h",
      "include/**.hpp",

      "platform/**.h",
      "platform/**.hpp",
      "platform/**.cpp",

      "src/**.h",
      "src/**.hpp",
      "src/**.cpp",

      -- ImGui & stb_image compiled directly into Engine
      "dep/imgui/*.h",
      "dep/imgui/*.hpp",
      "dep/imgui/*.cpp",
      "dep/stb_image.cpp"
   }

   includedirs {
      "include",
      "platform",
      "src",
      "dep",
      "dep/imgui"
   }

   filter "system:windows"
      systemversion "latest"

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
      runtime "Release"
      optimize "On"
      symbols "Off"