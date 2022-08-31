workspace "Raptor"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["GLFW"] = "Raptor/vendor/GLFW/include"
    IncludeDir["Glad"] = "Raptor/vendor/Glad/include"
    IncludeDir["ImGui"] = "Raptor/vendor/imgui"

    include "Raptor/vendor/GLFW"
    include "Raptor/vendor/GLAD"
    include "Raptor/vendor/imgui"

    project "Raptor"
        location "Raptor"
        kind "SharedLib"
        language "c++"
        staticruntime "off"

        targetdir ("bin/"..outputdir.."/%{prj.name}")
        objdir ("bin-int/"..outputdir.."/%{prj.name}")

        pchheader "rtpch.h"
        pchsource "Raptor/src/rtpch.cpp"

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs
        {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.ImGui}"
        }

        links
        {
            "GLFW",
            "Glad",
            "ImGui",
            "opengl32.lib"
        }

        filter "system:windows"
            cppdialect "c++17"
            systemversion "latest"

            defines
            {
                "RT_PLATFORM_WINDOWS",
                "RT_BUILD_DLL",
                "GLFW_INCLUDE_NONE"
            }

            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} \"../bin/"..outputdir.."/Sandbox/\"")
            }

        filter "configurations:Debug"
            defines "RT_DEBUG"
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines "RT_RELEASE"
            runtime "Release"
            optimize "On"

        filter "configurations:Dist"
            defines "RT_DIST"
            runtime "Release"
            optimize "On"

    project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "c++"
        staticruntime "off"

        targetdir ("bin/"..outputdir.."/%{prj.name}")
        objdir ("bin-int/"..outputdir.."/%{prj.name}")

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs
        {
            "Raptor/vendor/spdlog/include",
            "Raptor/src"
        }

        links
        {
            "Raptor"
        }

        filter "system:windows"
            cppdialect "c++17"
            systemversion "latest"

            defines
            {
                "RT_PLATFORM_WINDOWS"
            }

        filter "configurations:Debug"
            defines "RT_DEBUG"
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines "RT_RELEASE"
            runtime "Release"
            optimize "On"

        filter "configurations:Dist"
            defines "RT_DIST"
            runtime "Release"
            optimize "On"
