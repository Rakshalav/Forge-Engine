workspace "Forge"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Editor" 

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus", "/utf-8" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

-- Global VCPKG paths (Updated to static triplet)
vcpkg_root = os.getenv("VCPKG_ROOT") or "C:/vcpkg"
vcpkg_installed = vcpkg_root .. "/installed/x64-windows-static"

includedirs {
   vcpkg_installed .. "/include"
}

include "engine/Build-Engine.lua"
include "tools/editor/Build-Editor.lua"