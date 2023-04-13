local RaptorRootDir = '../../../../'

include (RaptorRootDir .. "Custom_premake.lua")

workspace "Sandbox"
    architecture "x86_64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
	{
		"MultiProcessorCompile"
	}

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


	project "Sandbox"
		kind "SharedLib"
		language "C#"
		dotnetframework "4.7.2"
	
		targetdir ("Binaries")
		objdir ("Intermediate")
	
		files
		{
			"Source/**.cs",
			"Source/Properties/**.cs"
		}
	
		links
		{
			"Raptor-ScriptCore"
		}
	
	
		filter "configurations:Debug"
			optimize "off"
			symbols "Default"
	
		filter "configurations:Release"
			optimize "on"
			symbols "Default"
	
		filter "configurations:Dist"
			optimize "full"
			symbols "off"
	
group "Raptor"
	include (RaptorRootDir .. "Raptor-ScriptCore")
group ""