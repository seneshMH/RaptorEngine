project "Raptor-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("../Raptor-Editor/resources/scripts")
	objdir ("../Raptor-Editor/resources/scripts/intermediate")

	files
	{
		"Source/**.cs",
		"Source/Properties/**.cs"
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