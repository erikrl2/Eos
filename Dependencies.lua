
-- Eos Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Eos/vendor/spdlog/include"
IncludeDir["stb_image"] = "%{wks.location}/Eos/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Eos/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/Eos/vendor/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/Eos/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Eos/vendor/Glad/include"
IncludeDir["Imgui"] = "%{wks.location}/Eos/vendor/imgui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Eos/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/Eos/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Eos/vendor/entt/include"
IncludeDir["shaderc"] = "%{wks.location}/Eos/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Eos/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
