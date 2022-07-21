include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Eos"
	architecture "x86_64"
	startproject "EosEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Eos/vendor/Box2D"
	include "Eos/vendor/GLFW"
	include "Eos/vendor/Glad"
	include "Eos/vendor/imgui"
	include "Eos/vendor/yaml-cpp"
group ""

group "Core"
	include "Eos"
	include "Eos-ScriptCore"
group ""

group "Tools"
	include "EosEditor"
group ""

group "Misc"
	include "Sandbox"
group ""
