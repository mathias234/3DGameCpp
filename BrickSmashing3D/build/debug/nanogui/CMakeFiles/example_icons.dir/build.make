# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.3\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.3\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = G:\dev\BrickSmashing3D\BrickSmashing3D

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug

# Include any dependencies generated for this target.
include nanogui/CMakeFiles/example_icons.dir/depend.make

# Include the progress variables for this target.
include nanogui/CMakeFiles/example_icons.dir/progress.make

# Include the compile flags for this target's objects.
include nanogui/CMakeFiles/example_icons.dir/flags.make

nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj: nanogui/CMakeFiles/example_icons.dir/flags.make
nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj: nanogui/CMakeFiles/example_icons.dir/includes_CXX.rsp
nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj: ../../nanogui/src/example_icons.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj"
	cd /d G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\nanogui && C:\PROGRA~2\MINGW-~1\I686-7~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\example_icons.dir\src\example_icons.cpp.obj -c G:\dev\BrickSmashing3D\BrickSmashing3D\nanogui\src\example_icons.cpp

nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_icons.dir/src/example_icons.cpp.i"
	cd /d G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\nanogui && C:\PROGRA~2\MINGW-~1\I686-7~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E G:\dev\BrickSmashing3D\BrickSmashing3D\nanogui\src\example_icons.cpp > CMakeFiles\example_icons.dir\src\example_icons.cpp.i

nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_icons.dir/src/example_icons.cpp.s"
	cd /d G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\nanogui && C:\PROGRA~2\MINGW-~1\I686-7~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S G:\dev\BrickSmashing3D\BrickSmashing3D\nanogui\src\example_icons.cpp -o CMakeFiles\example_icons.dir\src\example_icons.cpp.s

nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj.requires:

.PHONY : nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj.requires

nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj.provides: nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj.requires
	$(MAKE) -f nanogui\CMakeFiles\example_icons.dir\build.make nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj.provides.build
.PHONY : nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj.provides

nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj.provides.build: nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj


# Object files for target example_icons
example_icons_OBJECTS = \
"CMakeFiles/example_icons.dir/src/example_icons.cpp.obj"

# External object files for target example_icons
example_icons_EXTERNAL_OBJECTS =

nanogui/example_icons.exe: nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj
nanogui/example_icons.exe: nanogui/CMakeFiles/example_icons.dir/build.make
nanogui/example_icons.exe: nanogui/libnanogui.dll.a
nanogui/example_icons.exe: nanogui/CMakeFiles/example_icons.dir/linklibs.rsp
nanogui/example_icons.exe: nanogui/CMakeFiles/example_icons.dir/objects1.rsp
nanogui/example_icons.exe: nanogui/CMakeFiles/example_icons.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable example_icons.exe"
	cd /d G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\nanogui && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\example_icons.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
nanogui/CMakeFiles/example_icons.dir/build: nanogui/example_icons.exe

.PHONY : nanogui/CMakeFiles/example_icons.dir/build

nanogui/CMakeFiles/example_icons.dir/requires: nanogui/CMakeFiles/example_icons.dir/src/example_icons.cpp.obj.requires

.PHONY : nanogui/CMakeFiles/example_icons.dir/requires

nanogui/CMakeFiles/example_icons.dir/clean:
	cd /d G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\nanogui && $(CMAKE_COMMAND) -P CMakeFiles\example_icons.dir\cmake_clean.cmake
.PHONY : nanogui/CMakeFiles/example_icons.dir/clean

nanogui/CMakeFiles/example_icons.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" G:\dev\BrickSmashing3D\BrickSmashing3D G:\dev\BrickSmashing3D\BrickSmashing3D\nanogui G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\nanogui G:\dev\BrickSmashing3D\BrickSmashing3D\build\debug\nanogui\CMakeFiles\example_icons.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : nanogui/CMakeFiles/example_icons.dir/depend

