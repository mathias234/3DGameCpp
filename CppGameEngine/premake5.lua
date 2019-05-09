workspace "CppGameEngine"
architecture "x64"
startproject "Sandbox"

configurations
{
    "Debug",
    "Release",
    "Dist"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "../Dependencies/GLFW/include"
IncludeDir["glew"] = "../Dependencies/glew-2.1.0/include"
IncludeDir["glm"] = "../Dependencies/glm-0.9.9-a2"

include "/Dependencies/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"
