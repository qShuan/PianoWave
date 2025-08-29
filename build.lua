workspace "Piano Wave"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "PianoWave"

project "PianoWave"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
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
            "flac.lib",
            "vorbisenc.lib",
            "vorbisfile.lib",
            "vorbis.lib",
            "ogg.lib",
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
            "flac.lib",
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