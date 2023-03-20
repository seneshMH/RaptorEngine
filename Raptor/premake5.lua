include "Dependencies.lua"

workspace "Raptor"
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

    group "_Dependencies"
        include "Raptor/vendor/GLFW"
        include "Raptor/vendor/GLAD"
        include "Raptor/vendor/imgui"
        include "Raptor/vendor/yaml-cpp"
        include "Raptor/vendor/Box2D"
    group ""
  
    group "Core"
        include "Raptor"
        include "Raptor-ScriptCore"
    group ""

    group "Tools"
        include "Raptor-Editor"
    group ""

    group "Mics"
        include "Sandbox"
    group ""