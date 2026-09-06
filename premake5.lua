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
IncludeDir["Glad"] = "Boozy/vendor/Glad/include"
IncludeDir["ImGui"] = "Boozy/vendor/imgui"
IncludeDir["glm"] = "Boozy/vendor/glm"
IncludeDir["stb_image"] = "Boozy/vendor/stb_image"

include "Boozy/vendor/Glad"

project "GLFW"
    location "Boozy/vendor"
    kind "StaticLib"
    language "C"
    staticruntime "on"


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
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"

project "ImGui"
    location "Boozy/vendor"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Boozy/vendor/imgui/imgui.cpp",
        "Boozy/vendor/imgui/imgui_draw.cpp",
        "Boozy/vendor/imgui/imgui_tables.cpp",
        "Boozy/vendor/imgui/imgui_widgets.cpp",
        "Boozy/vendor/imgui/imgui_demo.cpp"
    }

    includedirs
    {
        "Boozy/vendor/imgui",
        "Boozy/vendor/imgui/backends",
        "Boozy/vendor/GLFW/include"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"

project "Boozy"
    location "Boozy"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "bzpch.h"
    pchsource "Boozy/src/bzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "BZ_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines { "BZ_CONFIG_DEBUG", "BZ_ENABLE_ASSERTS" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "BZ_CONFIG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "BZ_CONFIG_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "Boozy/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Boozy",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "BZ_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "BZ_CONFIG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "BZ_CONFIG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "BZ_CONFIG_DIST"
        runtime "Release"
        optimize "on"
