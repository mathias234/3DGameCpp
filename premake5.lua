workspace "CppGameEngine"
	architecture "x64"
	startproject "Engine"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glfw"] = "Dependencies/glfw/include"
IncludeDir["glad"] = "Dependencies/Glad/include"
IncludeDir["glm"] = "Dependencies/glm"

include "Dependencies/glfw"
include "Dependencies/Glad"


project "Engine"
	location "Engine"
    kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	characterset "MBCS"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp"
    }
    
    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    includedirs
	{
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}"
    }
    
    links 
	{ 
		"glfw",
		"Glad",
		"opengl32.lib"
	}

    filter "system:windows"
		systemversion "latest"
		defines
		{
			"GLFW_INCLUDE_NONE"
        }
        
    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"