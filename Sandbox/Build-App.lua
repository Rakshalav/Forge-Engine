project "Sandbox"
    kind "ConsoleApp"
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
        "Source",
        "../Forge",
        "../Forge/Platform",
        "../Forge/Source",
        "../Forge/Vendor/include"
    }

    libdirs {
        "../Forge/Vendor/lib",
        "../Binaries/" .. OutputDir .. "/Forge"
    }

    filter "files:Vendor/include/**.c"
        language "C"

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

       links { 
            "Forge",
            "glfw3", 
            "opengl32", 
            "user32", 
            "gdi32", 
            "shell32" 
        }

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