project "Raptor"
    kind "StaticLib"
    language "c++"
    cppdialect "c++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
    objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    pchheader "rtpch.h"
    pchsource "src/rtpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
        "vendor/ImGizmo/ImGuizmo.h",
        "vendor/ImGizmo/ImGuizmo.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.mono}",
        "%{IncludeDir.filewatch}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp",
        "Box2D",
        "opengl32.lib",
        "%{Library.mono}"
    }

    filter "files:vendor/ImGizmo/**.cpp"
    flags {"NoPCH"}

    filter "system:windows"
        systemversion "latest"

        defines
        {
           
        }

        links
        {
            "%{Library.winsock}",
            "%{Library.winmm}",
            "%{Library.winversion}",
            "%{Library.bcrypt}"
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
  