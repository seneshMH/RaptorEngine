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
    IncludeDir["glm"] = "Raptor/vendor/glm"

    include "Raptor/vendor/GLFW"
    include "Raptor/vendor/GLAD"
    include "Raptor/vendor/imgui"

    project "Raptor"
        location "Raptor"
        kind "StaticLib"
        language "c++"
        cppdialect "c++17"
        staticruntime "on"

        targetdir ("bin/"..outputdir.."/%{prj.name}")
        objdir ("bin-int/"..outputdir.."/%{prj.name}")

        pchheader "rtpch.h"
        pchsource "Raptor/src/rtpch.cpp"

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/vendor/glm/glm/**.hpp",
            "%{prj.name}/vendor/glm/glm/**.inl"
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS"
        }

        includedirs
        {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}"
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
                "RT_PLATFORM_WINDOWS",
                "RT_BUILD_DLL",
                "GLFW_INCLUDE_NONE"
            }


        filter "configurations:Debug"
            defines "RT_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "RT_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "RT_DIST"
            runtime "Release"
            optimize "on"

    project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "c++"
        cppdialect "c++17"
        staticruntime "on"

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
            "Raptor/src",
            "%{IncludeDir.glm}"
        }

        links
        {
            "Raptor"
        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "RT_PLATFORM_WINDOWS"
            }

        filter "configurations:Debug"
            defines "RT_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "RT_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "RT_DIST"
            runtime "Release"
            optimize "on"
