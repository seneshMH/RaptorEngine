workspace "Raptor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Raptor"
    location "Raptor"
    kind "SharedLib"
    language "c++"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RT_PLATFORM_WINDOWS",
            "RT_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/Sandbox")
        }

    filter "configurations:Debug"
        defines "RT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RT_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "RT_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "c++"

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
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RT_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "RT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RT_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "RT_DIST"
        optimize "On"


