# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\mimmo\Documents\C++\ELDAR

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\mimmo\Documents\C++\ELDAR\build

# Include any dependencies generated for this target.
include CMakeFiles/eldar.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/eldar.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/eldar.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/eldar.dir/flags.make

CMakeFiles/eldar.dir/src/main.cpp.obj: CMakeFiles/eldar.dir/flags.make
CMakeFiles/eldar.dir/src/main.cpp.obj: CMakeFiles/eldar.dir/includes_CXX.rsp
CMakeFiles/eldar.dir/src/main.cpp.obj: ../src/main.cpp
CMakeFiles/eldar.dir/src/main.cpp.obj: CMakeFiles/eldar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mimmo\Documents\C++\ELDAR\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/eldar.dir/src/main.cpp.obj"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/eldar.dir/src/main.cpp.obj -MF CMakeFiles\eldar.dir\src\main.cpp.obj.d -o CMakeFiles\eldar.dir\src\main.cpp.obj -c C:\Users\mimmo\Documents\C++\ELDAR\src\main.cpp

CMakeFiles/eldar.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/eldar.dir/src/main.cpp.i"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mimmo\Documents\C++\ELDAR\src\main.cpp > CMakeFiles\eldar.dir\src\main.cpp.i

CMakeFiles/eldar.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/eldar.dir/src/main.cpp.s"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\mimmo\Documents\C++\ELDAR\src\main.cpp -o CMakeFiles\eldar.dir\src\main.cpp.s

CMakeFiles/eldar.dir/src/device_mgr.cpp.obj: CMakeFiles/eldar.dir/flags.make
CMakeFiles/eldar.dir/src/device_mgr.cpp.obj: CMakeFiles/eldar.dir/includes_CXX.rsp
CMakeFiles/eldar.dir/src/device_mgr.cpp.obj: ../src/device_mgr.cpp
CMakeFiles/eldar.dir/src/device_mgr.cpp.obj: CMakeFiles/eldar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mimmo\Documents\C++\ELDAR\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/eldar.dir/src/device_mgr.cpp.obj"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/eldar.dir/src/device_mgr.cpp.obj -MF CMakeFiles\eldar.dir\src\device_mgr.cpp.obj.d -o CMakeFiles\eldar.dir\src\device_mgr.cpp.obj -c C:\Users\mimmo\Documents\C++\ELDAR\src\device_mgr.cpp

CMakeFiles/eldar.dir/src/device_mgr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/eldar.dir/src/device_mgr.cpp.i"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mimmo\Documents\C++\ELDAR\src\device_mgr.cpp > CMakeFiles\eldar.dir\src\device_mgr.cpp.i

CMakeFiles/eldar.dir/src/device_mgr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/eldar.dir/src/device_mgr.cpp.s"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\mimmo\Documents\C++\ELDAR\src\device_mgr.cpp -o CMakeFiles\eldar.dir\src\device_mgr.cpp.s

CMakeFiles/eldar.dir/src/json_mgr.cpp.obj: CMakeFiles/eldar.dir/flags.make
CMakeFiles/eldar.dir/src/json_mgr.cpp.obj: CMakeFiles/eldar.dir/includes_CXX.rsp
CMakeFiles/eldar.dir/src/json_mgr.cpp.obj: ../src/json_mgr.cpp
CMakeFiles/eldar.dir/src/json_mgr.cpp.obj: CMakeFiles/eldar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mimmo\Documents\C++\ELDAR\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/eldar.dir/src/json_mgr.cpp.obj"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/eldar.dir/src/json_mgr.cpp.obj -MF CMakeFiles\eldar.dir\src\json_mgr.cpp.obj.d -o CMakeFiles\eldar.dir\src\json_mgr.cpp.obj -c C:\Users\mimmo\Documents\C++\ELDAR\src\json_mgr.cpp

CMakeFiles/eldar.dir/src/json_mgr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/eldar.dir/src/json_mgr.cpp.i"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mimmo\Documents\C++\ELDAR\src\json_mgr.cpp > CMakeFiles\eldar.dir\src\json_mgr.cpp.i

CMakeFiles/eldar.dir/src/json_mgr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/eldar.dir/src/json_mgr.cpp.s"
	C:\Qt\Tools\mingw810_64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\mimmo\Documents\C++\ELDAR\src\json_mgr.cpp -o CMakeFiles\eldar.dir\src\json_mgr.cpp.s

# Object files for target eldar
eldar_OBJECTS = \
"CMakeFiles/eldar.dir/src/main.cpp.obj" \
"CMakeFiles/eldar.dir/src/device_mgr.cpp.obj" \
"CMakeFiles/eldar.dir/src/json_mgr.cpp.obj"

# External object files for target eldar
eldar_EXTERNAL_OBJECTS =

eldar.exe: CMakeFiles/eldar.dir/src/main.cpp.obj
eldar.exe: CMakeFiles/eldar.dir/src/device_mgr.cpp.obj
eldar.exe: CMakeFiles/eldar.dir/src/json_mgr.cpp.obj
eldar.exe: CMakeFiles/eldar.dir/build.make
eldar.exe: CMakeFiles/eldar.dir/linklibs.rsp
eldar.exe: CMakeFiles/eldar.dir/objects1.rsp
eldar.exe: CMakeFiles/eldar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\mimmo\Documents\C++\ELDAR\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable eldar.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\eldar.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/eldar.dir/build: eldar.exe
.PHONY : CMakeFiles/eldar.dir/build

CMakeFiles/eldar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\eldar.dir\cmake_clean.cmake
.PHONY : CMakeFiles/eldar.dir/clean

CMakeFiles/eldar.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\mimmo\Documents\C++\ELDAR C:\Users\mimmo\Documents\C++\ELDAR C:\Users\mimmo\Documents\C++\ELDAR\build C:\Users\mimmo\Documents\C++\ELDAR\build C:\Users\mimmo\Documents\C++\ELDAR\build\CMakeFiles\eldar.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/eldar.dir/depend
