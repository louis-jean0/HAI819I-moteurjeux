# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external

# Include any dependencies generated for this target.
include glfw-3.1.2/tests/CMakeFiles/msaa.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include glfw-3.1.2/tests/CMakeFiles/msaa.dir/compiler_depend.make

# Include the progress variables for this target.
include glfw-3.1.2/tests/CMakeFiles/msaa.dir/progress.make

# Include the compile flags for this target's objects.
include glfw-3.1.2/tests/CMakeFiles/msaa.dir/flags.make

glfw-3.1.2/tests/CMakeFiles/msaa.dir/msaa.c.o: glfw-3.1.2/tests/CMakeFiles/msaa.dir/flags.make
glfw-3.1.2/tests/CMakeFiles/msaa.dir/msaa.c.o: glfw-3.1.2/tests/msaa.c
glfw-3.1.2/tests/CMakeFiles/msaa.dir/msaa.c.o: glfw-3.1.2/tests/CMakeFiles/msaa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw-3.1.2/tests/CMakeFiles/msaa.dir/msaa.c.o"
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT glfw-3.1.2/tests/CMakeFiles/msaa.dir/msaa.c.o -MF CMakeFiles/msaa.dir/msaa.c.o.d -o CMakeFiles/msaa.dir/msaa.c.o -c /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests/msaa.c

glfw-3.1.2/tests/CMakeFiles/msaa.dir/msaa.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/msaa.dir/msaa.c.i"
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests/msaa.c > CMakeFiles/msaa.dir/msaa.c.i

glfw-3.1.2/tests/CMakeFiles/msaa.dir/msaa.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/msaa.dir/msaa.c.s"
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests/msaa.c -o CMakeFiles/msaa.dir/msaa.c.s

glfw-3.1.2/tests/CMakeFiles/msaa.dir/__/deps/getopt.c.o: glfw-3.1.2/tests/CMakeFiles/msaa.dir/flags.make
glfw-3.1.2/tests/CMakeFiles/msaa.dir/__/deps/getopt.c.o: glfw-3.1.2/deps/getopt.c
glfw-3.1.2/tests/CMakeFiles/msaa.dir/__/deps/getopt.c.o: glfw-3.1.2/tests/CMakeFiles/msaa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object glfw-3.1.2/tests/CMakeFiles/msaa.dir/__/deps/getopt.c.o"
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT glfw-3.1.2/tests/CMakeFiles/msaa.dir/__/deps/getopt.c.o -MF CMakeFiles/msaa.dir/__/deps/getopt.c.o.d -o CMakeFiles/msaa.dir/__/deps/getopt.c.o -c /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/deps/getopt.c

glfw-3.1.2/tests/CMakeFiles/msaa.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/msaa.dir/__/deps/getopt.c.i"
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/deps/getopt.c > CMakeFiles/msaa.dir/__/deps/getopt.c.i

glfw-3.1.2/tests/CMakeFiles/msaa.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/msaa.dir/__/deps/getopt.c.s"
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/deps/getopt.c -o CMakeFiles/msaa.dir/__/deps/getopt.c.s

# Object files for target msaa
msaa_OBJECTS = \
"CMakeFiles/msaa.dir/msaa.c.o" \
"CMakeFiles/msaa.dir/__/deps/getopt.c.o"

# External object files for target msaa
msaa_EXTERNAL_OBJECTS =

glfw-3.1.2/tests/msaa: glfw-3.1.2/tests/CMakeFiles/msaa.dir/msaa.c.o
glfw-3.1.2/tests/msaa: glfw-3.1.2/tests/CMakeFiles/msaa.dir/__/deps/getopt.c.o
glfw-3.1.2/tests/msaa: glfw-3.1.2/tests/CMakeFiles/msaa.dir/build.make
glfw-3.1.2/tests/msaa: glfw-3.1.2/src/libglfw3.a
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/librt.a
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/libm.so
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/libX11.so
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/libXrandr.so
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/libXinerama.so
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/libXi.so
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/libXcursor.so
glfw-3.1.2/tests/msaa: /usr/lib/x86_64-linux-gnu/libGL.so
glfw-3.1.2/tests/msaa: glfw-3.1.2/tests/CMakeFiles/msaa.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable msaa"
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/msaa.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw-3.1.2/tests/CMakeFiles/msaa.dir/build: glfw-3.1.2/tests/msaa
.PHONY : glfw-3.1.2/tests/CMakeFiles/msaa.dir/build

glfw-3.1.2/tests/CMakeFiles/msaa.dir/clean:
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -P CMakeFiles/msaa.dir/cmake_clean.cmake
.PHONY : glfw-3.1.2/tests/CMakeFiles/msaa.dir/clean

glfw-3.1.2/tests/CMakeFiles/msaa.dir/depend:
	cd /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests /home/louis/Documents/HAI819I-moteurjeux/TPCamera/external/glfw-3.1.2/tests/CMakeFiles/msaa.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw-3.1.2/tests/CMakeFiles/msaa.dir/depend

