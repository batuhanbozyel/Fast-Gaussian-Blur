workspace "Pixery-Gaussian-Blur"
	architecture "x86_64"
	startproject "GaussianBlurFilter"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "vendor/GLFW"
	include "vendor/Glad"
	include "vendor/imgui"
group ""

project "GaussianBlurFilter"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" ..outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Core/pch.cpp"

	files
	{
		"Core/**.h",
		"Core/**.cpp",

		"Graphics/**.h",
		"Graphics/**.cpp",

		"UI/**.h",
		"UI/**.cpp",
		
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",

		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"vendor",
		"%{wks.location}/vendor/GLFW/include",
		"%{wks.location}/vendor/Glad/include",
		"%{wks.location}/vendor/glm",
		"%{wks.location}/vendor/spdlog/include",
		"%{wks.location}/vendor/imgui"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PDEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"