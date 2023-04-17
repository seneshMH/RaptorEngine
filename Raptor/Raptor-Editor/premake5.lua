project "Raptor-Editor"
    kind "ConsoleApp"
    language "c++"
    cppdialect "c++17"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
    objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Raptor/vendor/spdlog/include",
        "%{wks.location}/Raptor/src",
        "%{wks.location}/Raptor/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.filewatch}"
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