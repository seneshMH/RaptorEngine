workspace "Raptor"
    architecture "x86_64"
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
    IncludeDir["stb_image"] = "Raptor/vendor/stb_image"
    IncludeDir["entt"] = "Raptor/vendor/entt/include"
    IncludeDir["yaml_cpp"] = "Raptor/vendor/yaml-cpp/include"
    IncludeDir["ImGuizmo"] = "Raptor/vendor/ImGizmo"
    IncludeDir["Box2D"] = "Raptor/vendor/Box2D/include"

    group "Dependencies"
        include "Raptor/vendor/GLFW"
        include "Raptor/vendor/GLAD"
        include "Raptor/vendor/imgui"
        include "Raptor/vendor/yaml-cpp"
        include "Raptor/vendor/Box2D"
    group ""

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
            "%{prj.name}/vendor/stb_image/**.h",
            "%{prj.name}/vendor/stb_image/**.cpp",
            "%{prj.name}/vendor/glm/glm/**.hpp",
            "%{prj.name}/vendor/glm/glm/**.inl",
            "%{prj.name}/vendor/ImGizmo/ImGuizmo.h",
            "%{prj.name}/vendor/ImGizmo/ImGuizmo.cpp"
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "GLFW_INCLUDE_NONE"
        }

        includedirs
        {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb_image}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.yaml_cpp}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.Box2D}"
        }

        links
        {
            "GLFW",
            "Glad",
            "ImGui",
            "yaml-cpp",
            "Box2D",
            "opengl32.lib"
        }

        filter "files:Raptor/vendor/ImGizmo/**.cpp"
        flags {"NoPCH"}

        filter "system:windows"
            systemversion "latest"

            defines
            {
               
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
            "Raptor/vendor",
            "%{IncludeDir.glm}"
        }

        links
        {
            "Raptor"
        }

        filter "system:windows"
            systemversion "latest"

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

    project "Raptor-Editor"
        location "Raptor-Editor"
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
            "Raptor/vendor",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.ImGuizmo}"
        }

        links
        {
            "Raptor"
        }

        filter "system:windows"
            systemversion "latest"

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

