local success, err = pcall(require, "export-compile-commands")
if not success then
    print("Note: export-compile-commands module not found. compile_commands.json will not be generated.")
end

local CXX_STANDARD = "C++23"

workspace "Piano Wave"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "PianoWave"

project "PianoWave"
    kind "ConsoleApp"
    language "C++"
    cppdialect(CXX_STANDARD)
    staticruntime "Off"
    targetdir "bin/%{cfg.architecture}/%{cfg.buildcfg}"
    objdir    "obj/%{cfg.architecture}/%{cfg.buildcfg}"
    flags { "MultiProcessorCompile" }

    files {
        "src/**.cpp",
        "src/**.h",
        "include/**.cpp",
        "include/**.hpp",
        "include/**.h"
    }

    includedirs {
        "src",
        "include",
        "include/utils",
        "include/imgui"
    }

    libdirs {
        "lib"
    }

    defines {
        "SFML_STATIC"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        links {
            "sfml-system-s-d",
            "sfml-window-s-d",
            "sfml-graphics-s-d",
            "sfml-audio-s-d.lib",
            "FLACd.lib",
            "vorbisencd.lib",
            "vorbisfiled.lib",
            "vorbisd.lib",
            "oggd.lib",
            "openal32.lib",
            "opengl32",
            "winmm"
        }
        defines{
            "_DEBUG",
            "_CONSOLE"
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        links {
            "sfml-system-s",
            "sfml-window-s",
            "sfml-graphics-s",
            "sfml-audio-s.lib",
            "FLAC.lib",
            "vorbisenc.lib",
            "vorbisfile.lib",
            "vorbis.lib",
            "ogg.lib",
            "openal32.lib",
            "opengl32",
            "winmm"
        }
        defines{
            "NDEBUG",
            "_CONSOLE"
        }

if success then
    local clangd_file = io.open(".clangd", "w")
    if clangd_file then
        clangd_file:write("CompileFlags:\n")
        clangd_file:write("  Add:\n")
        clangd_file:write("    - \"-std=" .. CXX_STANDARD:lower() .. "\"\n")
        clangd_file:close()
    end
end
