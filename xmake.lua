set_project("Boozy")
set_xmakever("3.1.2")
set_policy("package.requires_lock", true)

add_requires("glfw 3.4")
add_requires("glm 1.0.3")
add_requires("imgui v1.92.8-docking", {configs = {glfw = true, opengl3 = true}})
add_requires("assimp v6.0.5", {configs = {no_export = true}})
add_requires("stb 2026.03.18")
add_requires("glad v2.0.8", {configs = {gl_version = "4.6", profile = "core"}})

add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    add_defines("BZ_PLATFORM_WINDOWS", "UNICODE", "_UNICODE")
elseif is_plat("linux") then
    add_defines("BZ_PLATFORM_LINUX")
    add_cxxflags("-Wno-unknown-pragmas")
end

target("Boozy")
    set_kind("static")
    set_languages("c++20")
    set_warnings("all")
    set_pcxxheader("Boozy/src/bzpch.h")
    add_packages("glfw", "glad", "imgui", "glm", "assimp", "stb", {public = true})
    add_files("Boozy/src/**.cpp")
    add_includedirs("Boozy/src", {public = true})
    if is_mode("debug") then
        add_defines("BZ_CONFIG_DEBUG", "BZ_ENABLE_ASSERTS")
    else
        add_defines("BZ_CONFIG_RELEASE")
    end
    if is_plat("windows") then add_syslinks("opengl32") end

target("Sandbox")
    set_kind("binary")
    set_languages("c++20")
    set_warnings("all")
    add_deps("Boozy")
    add_files("Sandbox/src/**.cpp")
    set_rundir("$(projectdir)/Sandbox")
    