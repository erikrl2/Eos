project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	debugdir "%{wks.location}/EosEditor"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Eos/src",
		"%{wks.location}/Eos/vendor",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Box2D}"
	}

	links
	{
		"Eos"
	}
		
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "EOS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EOS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EOS_DIST"
		runtime "Release"
		optimize "on"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"
