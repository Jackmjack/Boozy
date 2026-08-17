workspace "Boozy"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["GLFW"] = "Boozy/vendor/GLFW/include"

project "GLFW"
    location "Boozy/vendor"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Boozy/vendor/GLFW/include/GLFW/glfw3.h",
        "Boozy/vendor/GLFW/include/GLFW/glfw3native.h",
        "Boozy/vendor/GLFW/src/context.c",
        "Boozy/vendor/GLFW/src/init.c",
        "Boozy/vendor/GLFW/src/input.c",
        "Boozy/vendor/GLFW/src/monitor.c",
        "Boozy/vendor/GLFW/src/platform.c",
        "Boozy/vendor/GLFW/src/vulkan.c",
        "Boozy/vendor/GLFW/src/window.c",
        "Boozy/vendor/GLFW/src/egl_context.c",
        "Boozy/vendor/GLFW/src/osmesa_context.c",
        "Boozy/vendor/GLFW/src/null_init.c",
        "Boozy/vendor/GLFW/src/null_monitor.c",
        "Boozy/vendor/GLFW/src/null_window.c",
        "Boozy/vendor/GLFW/src/null_joystick.c"
    }

    includedirs
    {
        "Boozy/vendor"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "Off"

        files
        {
            "Boozy/vendor/GLFW/src/win32_init.c",
            "Boozy/vendor/GLFW/src/win32_joystick.c",
            "Boozy/vendor/GLFW/src/win32_module.c",
            "Boozy/vendor/GLFW/src/win32_monitor.c",
            "Boozy/vendor/GLFW/src/win32_time.c",
            "Boozy/vendor/GLFW/src/win32_thread.c",
            "Boozy/vendor/GLFW/src/win32_window.c",
            "Boozy/vendor/GLFW/src/wgl_context.c"
        }

        defines
        {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        symbols "On"
        optimize "On"

    filter "configurations:Dist"
        runtime "Release"
        symbols "On"

project "Boozy"
    location "Boozy"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "bzpch.h"
    pchsource "Boozy/src/bzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib",
        "gdi32.lib",
        "user32.lib",
        "shell32.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "Off"
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
        defines "BZ_CONFIG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "BZ_CONFIG_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "BZ_CONFIG_DIST"
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
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "BZ_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "BZ_CONFIG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "BZ_CONFIG_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "BZ_CONFIG_DIST"
        symbols "On"
