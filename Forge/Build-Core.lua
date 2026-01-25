project "Forge"
   kind "StaticLib"
   language "C++"
   cppdialect "C++23"
   staticruntime "off"

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   files { 
        "Platform/**.h",
        "Platform/**.hpp",
        "Platform/**.cpp",
    
        "Source/**.h",
        "Source/**.cpp",
        "Source/**.hpp",

        "Vendor/include/**.h",
        "Vendor/include/**.c",
        "Vendor/include/**.hpp",
        "Vendor/include/**.cpp",
        "Vendor/include/**.inl"
    }

   includedirs {
        "Platform",
        "Source",
        "Vendor/include"
   }

   filter "files:Vendor/include/**.c"
        language "C"

   filter "system:windows"
       systemversion "latest"
       defines { }

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