VULKAN_SDK = os.getenv("VULKAN_SDK")

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
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

--IncludeDir["shaderc"] = "%{wks.location}/Hazel/vendor/shaderc/include"
--IncludeDir["SPIRV_Cross"] = "%{wks.location}/Hazel/vendor/SPIRV-Cross"


LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Raptor/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{VULKAN_SDK}/Lib"


Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

--Windows only
Library["winsock"] = "Ws2_32.lib"
Library["winmm"] = "Winmm.lib"
Library["winversion"] = "Version.lib"
Library["bcrypt"] = "Bcrypt.lib"