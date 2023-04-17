IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Raptor/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Raptor/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Raptor/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Raptor/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Raptor/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Raptor/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Raptor/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Raptor/vendor/ImGizmo"
IncludeDir["Box2D"] = "%{wks.location}/Raptor/vendor/Box2D/include"
IncludeDir["mono"] = "%{wks.location}/Raptor/vendor/mono/include"
IncludeDir["filewatch"] = "%{wks.location}/Raptor/vendor/filewatch"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Raptor/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

--Windows only
Library["winsock"] = "Ws2_32.lib"
Library["winmm"] = "Winmm.lib"
Library["winversion"] = "Version.lib"
Library["bcrypt"] = "Bcrypt.lib"