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
IncludeDir["assimp"] = "Boozy/vendor/assimp/include"
IncludeDir["assimpGen"] = "Boozy/vendor/assimp-generated"

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

project "assimp"
    location "Boozy/vendor"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Boozy/vendor/assimp/include/**.h",
        "Boozy/vendor/assimp/code/**.h",
        "Boozy/vendor/assimp/code/**.cpp",
        "Boozy/vendor/assimp/code/**.inl",
        "Boozy/vendor/assimp/contrib/**.h",
        "Boozy/vendor/assimp/contrib/**.c",
        "Boozy/vendor/assimp/contrib/**.cpp",
        "Boozy/vendor/assimp/contrib/**.inl",
    }

    removefiles
    {
        "Boozy/vendor/assimp/code/AssetLib/USD/**",
        "Boozy/vendor/assimp/code/AssetLib/VRML/**",

        "Boozy/vendor/assimp/contrib/draco/**",
        "Boozy/vendor/assimp/contrib/tinyusdz/**",
        "Boozy/vendor/assimp/contrib/meshlab/**",
        "Boozy/vendor/assimp/contrib/googletest/**",

        "Boozy/vendor/assimp/contrib/zlib/contrib/**",

        "Boozy/vendor/assimp/contrib/zip/test/**"
    }

    includedirs
    {
        "%{IncludeDir.assimpGen}",
        "%{IncludeDir.assimp}",
        "Boozy/vendor/assimp",
        "Boozy/vendor/assimp/code",
        "Boozy/vendor/assimp/contrib",
        "Boozy/vendor/assimp/contrib/zlib",
        "Boozy/vendor/assimp/contrib/zip/src",
        "Boozy/vendor/assimp/contrib/unzip",
        "Boozy/vendor/assimp/contrib/pugixml/src",
        "Boozy/vendor/assimp/contrib/utf8cpp/source",
        "Boozy/vendor/assimp/contrib/rapidjson/include",
        "Boozy/vendor/assimp/contrib/openddlparser/include",
        "Boozy/vendor/assimp/contrib/clipper",
        "Boozy/vendor/assimp/contrib/Open3DGC",
        "Boozy/vendor/assimp/contrib/poly2tri",
        "Boozy/vendor/assimp/contrib/stb",
    }

    defines
    {
        "ASSIMP_BUILD_NO_3DS_IMPORTER",
        "ASSIMP_BUILD_NO_3D_IMPORTER",
        "ASSIMP_BUILD_NO_3MF_IMPORTER",
        "ASSIMP_BUILD_NO_AC_IMPORTER",
        "ASSIMP_BUILD_NO_AMF_IMPORTER",
        "ASSIMP_BUILD_NO_ASE_IMPORTER",
        "ASSIMP_BUILD_NO_ASSBIN_IMPORTER",
        "ASSIMP_BUILD_NO_B3D_IMPORTER",
        "ASSIMP_BUILD_NO_BLEND_IMPORTER",
        "ASSIMP_BUILD_NO_BVH_IMPORTER",
        "ASSIMP_BUILD_NO_C4D_IMPORTER",
        "ASSIMP_BUILD_NO_COB_IMPORTER",
        "ASSIMP_BUILD_NO_COLLADA_IMPORTER",
        "ASSIMP_BUILD_NO_CSM_IMPORTER",
        "ASSIMP_BUILD_NO_DXF_IMPORTER",
        "ASSIMP_BUILD_NO_FBX_IMPORTER",
        "ASSIMP_BUILD_NO_GLTF_IMPORTER",
        "ASSIMP_BUILD_NO_GLTF1_IMPORTER",
        "ASSIMP_BUILD_NO_GLTF2_IMPORTER",
        "ASSIMP_BUILD_NO_HMP_IMPORTER",
        "ASSIMP_BUILD_NO_IFC_IMPORTER",
        "ASSIMP_BUILD_NO_IQM_IMPORTER",
        "ASSIMP_BUILD_NO_IRR_IMPORTER",
        "ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
        "ASSIMP_BUILD_NO_LWO_IMPORTER",
        "ASSIMP_BUILD_NO_LWS_IMPORTER",
        "ASSIMP_BUILD_NO_M3D_IMPORTER",
        "ASSIMP_BUILD_NO_MD2_IMPORTER",
        "ASSIMP_BUILD_NO_MD3_IMPORTER",
        "ASSIMP_BUILD_NO_MD5_IMPORTER",
        "ASSIMP_BUILD_NO_MDC_IMPORTER",
        "ASSIMP_BUILD_NO_MDL_IMPORTER",
        "ASSIMP_BUILD_NO_MMD_IMPORTER",
        "ASSIMP_BUILD_NO_MS3D_IMPORTER",
        "ASSIMP_BUILD_NO_NDO_IMPORTER",
        "ASSIMP_BUILD_NO_NFF_IMPORTER",
        "ASSIMP_BUILD_NO_OFF_IMPORTER",
        "ASSIMP_BUILD_NO_OGRE_IMPORTER",
        "ASSIMP_BUILD_NO_OPENGEX_IMPORTER",
        "ASSIMP_BUILD_NO_PLY_IMPORTER",
        "ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
        "ASSIMP_BUILD_NO_Q3D_IMPORTER",
        "ASSIMP_BUILD_NO_RAW_IMPORTER",
        "ASSIMP_BUILD_NO_SIB_IMPORTER",
        "ASSIMP_BUILD_NO_SMD_IMPORTER",
        "ASSIMP_BUILD_NO_STL_IMPORTER",
        "ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
        "ASSIMP_BUILD_NO_USD_IMPORTER",
        "ASSIMP_BUILD_NO_VRML_IMPORTER",
        "ASSIMP_BUILD_NO_X_IMPORTER",
        "ASSIMP_BUILD_NO_X3D_IMPORTER",
        "ASSIMP_BUILD_NO_XGL_IMPORTER",

        "ASSIMP_BUILD_NO_EXPORT",

        "OPENDDLPARSER_BUILD",
        "OPENDDL_STATIC_LIBARY",
        "P2T_STATIC_EXPORTS",

        "_CRT_SECURE_NO_WARNINGS",
    }

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/bigobj" }
        warnings "off"

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
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.assimpGen}",
        "%{IncludeDir.assimp}"
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
