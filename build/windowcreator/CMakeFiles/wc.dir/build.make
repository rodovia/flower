# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = /home/miguelrodovia/dev/flower

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/miguelrodovia/dev/flower/build

# Include any dependencies generated for this target.
include windowcreator/CMakeFiles/wc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include windowcreator/CMakeFiles/wc.dir/compiler_depend.make

# Include the progress variables for this target.
include windowcreator/CMakeFiles/wc.dir/progress.make

# Include the compile flags for this target's objects.
include windowcreator/CMakeFiles/wc.dir/flags.make

windowcreator/CMakeFiles/wc.dir/source/unix/window.c.o: windowcreator/CMakeFiles/wc.dir/flags.make
windowcreator/CMakeFiles/wc.dir/source/unix/window.c.o: /home/miguelrodovia/dev/flower/windowcreator/source/unix/window.c
windowcreator/CMakeFiles/wc.dir/source/unix/window.c.o: windowcreator/CMakeFiles/wc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object windowcreator/CMakeFiles/wc.dir/source/unix/window.c.o"
	cd /home/miguelrodovia/dev/flower/build/windowcreator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT windowcreator/CMakeFiles/wc.dir/source/unix/window.c.o -MF CMakeFiles/wc.dir/source/unix/window.c.o.d -o CMakeFiles/wc.dir/source/unix/window.c.o -c /home/miguelrodovia/dev/flower/windowcreator/source/unix/window.c

windowcreator/CMakeFiles/wc.dir/source/unix/window.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/wc.dir/source/unix/window.c.i"
	cd /home/miguelrodovia/dev/flower/build/windowcreator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/miguelrodovia/dev/flower/windowcreator/source/unix/window.c > CMakeFiles/wc.dir/source/unix/window.c.i

windowcreator/CMakeFiles/wc.dir/source/unix/window.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/wc.dir/source/unix/window.c.s"
	cd /home/miguelrodovia/dev/flower/build/windowcreator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/miguelrodovia/dev/flower/windowcreator/source/unix/window.c -o CMakeFiles/wc.dir/source/unix/window.c.s

windowcreator/CMakeFiles/wc.dir/source/unix/init.c.o: windowcreator/CMakeFiles/wc.dir/flags.make
windowcreator/CMakeFiles/wc.dir/source/unix/init.c.o: /home/miguelrodovia/dev/flower/windowcreator/source/unix/init.c
windowcreator/CMakeFiles/wc.dir/source/unix/init.c.o: windowcreator/CMakeFiles/wc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object windowcreator/CMakeFiles/wc.dir/source/unix/init.c.o"
	cd /home/miguelrodovia/dev/flower/build/windowcreator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT windowcreator/CMakeFiles/wc.dir/source/unix/init.c.o -MF CMakeFiles/wc.dir/source/unix/init.c.o.d -o CMakeFiles/wc.dir/source/unix/init.c.o -c /home/miguelrodovia/dev/flower/windowcreator/source/unix/init.c

windowcreator/CMakeFiles/wc.dir/source/unix/init.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/wc.dir/source/unix/init.c.i"
	cd /home/miguelrodovia/dev/flower/build/windowcreator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/miguelrodovia/dev/flower/windowcreator/source/unix/init.c > CMakeFiles/wc.dir/source/unix/init.c.i

windowcreator/CMakeFiles/wc.dir/source/unix/init.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/wc.dir/source/unix/init.c.s"
	cd /home/miguelrodovia/dev/flower/build/windowcreator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/miguelrodovia/dev/flower/windowcreator/source/unix/init.c -o CMakeFiles/wc.dir/source/unix/init.c.s

# Object files for target wc
wc_OBJECTS = \
"CMakeFiles/wc.dir/source/unix/window.c.o" \
"CMakeFiles/wc.dir/source/unix/init.c.o"

# External object files for target wc
wc_EXTERNAL_OBJECTS =

windowcreator/libwc.a: windowcreator/CMakeFiles/wc.dir/source/unix/window.c.o
windowcreator/libwc.a: windowcreator/CMakeFiles/wc.dir/source/unix/init.c.o
windowcreator/libwc.a: windowcreator/CMakeFiles/wc.dir/build.make
windowcreator/libwc.a: windowcreator/CMakeFiles/wc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libwc.a"
	cd /home/miguelrodovia/dev/flower/build/windowcreator && $(CMAKE_COMMAND) -P CMakeFiles/wc.dir/cmake_clean_target.cmake
	cd /home/miguelrodovia/dev/flower/build/windowcreator && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
windowcreator/CMakeFiles/wc.dir/build: windowcreator/libwc.a
.PHONY : windowcreator/CMakeFiles/wc.dir/build

windowcreator/CMakeFiles/wc.dir/clean:
	cd /home/miguelrodovia/dev/flower/build/windowcreator && $(CMAKE_COMMAND) -P CMakeFiles/wc.dir/cmake_clean.cmake
.PHONY : windowcreator/CMakeFiles/wc.dir/clean

windowcreator/CMakeFiles/wc.dir/depend:
	cd /home/miguelrodovia/dev/flower/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/miguelrodovia/dev/flower /home/miguelrodovia/dev/flower/windowcreator /home/miguelrodovia/dev/flower/build /home/miguelrodovia/dev/flower/build/windowcreator /home/miguelrodovia/dev/flower/build/windowcreator/CMakeFiles/wc.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : windowcreator/CMakeFiles/wc.dir/depend
