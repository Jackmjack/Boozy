workspace "Boozy"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Boozy"
    location "Boozy"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "BZ_PLATFORM_WINDOWS",
            "BZ_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{MKDIR} ../bin/" .. outputdir .. "/SandBox"),
            ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .."/SandBox/")
        }

    filter "configurations:Debug"
        defines "BZ_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "BZ_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "BZ_DIST"
        symbols "On"

project "SandBox"
    location "SandBox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "Boozy/src"
    }

    links
    {
        "Boozy"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "BZ_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "BZ_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "BZ_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "BZ_DIST"
        symbols "On"
