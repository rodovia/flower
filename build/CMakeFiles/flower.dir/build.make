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
include CMakeFiles/flower.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/flower.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/flower.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/flower.dir/flags.make

CMakeFiles/flower.dir/source/main.cpp.o: CMakeFiles/flower.dir/flags.make
CMakeFiles/flower.dir/source/main.cpp.o: /home/miguelrodovia/dev/flower/source/main.cpp
CMakeFiles/flower.dir/source/main.cpp.o: CMakeFiles/flower.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/flower.dir/source/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/flower.dir/source/main.cpp.o -MF CMakeFiles/flower.dir/source/main.cpp.o.d -o CMakeFiles/flower.dir/source/main.cpp.o -c /home/miguelrodovia/dev/flower/source/main.cpp

CMakeFiles/flower.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/flower.dir/source/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/miguelrodovia/dev/flower/source/main.cpp > CMakeFiles/flower.dir/source/main.cpp.i

CMakeFiles/flower.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/flower.dir/source/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/miguelrodovia/dev/flower/source/main.cpp -o CMakeFiles/flower.dir/source/main.cpp.s

CMakeFiles/flower.dir/source/html.cpp.o: CMakeFiles/flower.dir/flags.make
CMakeFiles/flower.dir/source/html.cpp.o: /home/miguelrodovia/dev/flower/source/html.cpp
CMakeFiles/flower.dir/source/html.cpp.o: CMakeFiles/flower.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/flower.dir/source/html.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/flower.dir/source/html.cpp.o -MF CMakeFiles/flower.dir/source/html.cpp.o.d -o CMakeFiles/flower.dir/source/html.cpp.o -c /home/miguelrodovia/dev/flower/source/html.cpp

CMakeFiles/flower.dir/source/html.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/flower.dir/source/html.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/miguelrodovia/dev/flower/source/html.cpp > CMakeFiles/flower.dir/source/html.cpp.i

CMakeFiles/flower.dir/source/html.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/flower.dir/source/html.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/miguelrodovia/dev/flower/source/html.cpp -o CMakeFiles/flower.dir/source/html.cpp.s

CMakeFiles/flower.dir/source/css/css.cpp.o: CMakeFiles/flower.dir/flags.make
CMakeFiles/flower.dir/source/css/css.cpp.o: /home/miguelrodovia/dev/flower/source/css/css.cpp
CMakeFiles/flower.dir/source/css/css.cpp.o: CMakeFiles/flower.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/flower.dir/source/css/css.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/flower.dir/source/css/css.cpp.o -MF CMakeFiles/flower.dir/source/css/css.cpp.o.d -o CMakeFiles/flower.dir/source/css/css.cpp.o -c /home/miguelrodovia/dev/flower/source/css/css.cpp

CMakeFiles/flower.dir/source/css/css.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/flower.dir/source/css/css.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/miguelrodovia/dev/flower/source/css/css.cpp > CMakeFiles/flower.dir/source/css/css.cpp.i

CMakeFiles/flower.dir/source/css/css.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/flower.dir/source/css/css.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/miguelrodovia/dev/flower/source/css/css.cpp -o CMakeFiles/flower.dir/source/css/css.cpp.s

CMakeFiles/flower.dir/source/css/css_color.cpp.o: CMakeFiles/flower.dir/flags.make
CMakeFiles/flower.dir/source/css/css_color.cpp.o: /home/miguelrodovia/dev/flower/source/css/css_color.cpp
CMakeFiles/flower.dir/source/css/css_color.cpp.o: CMakeFiles/flower.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/flower.dir/source/css/css_color.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/flower.dir/source/css/css_color.cpp.o -MF CMakeFiles/flower.dir/source/css/css_color.cpp.o.d -o CMakeFiles/flower.dir/source/css/css_color.cpp.o -c /home/miguelrodovia/dev/flower/source/css/css_color.cpp

CMakeFiles/flower.dir/source/css/css_color.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/flower.dir/source/css/css_color.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/miguelrodovia/dev/flower/source/css/css_color.cpp > CMakeFiles/flower.dir/source/css/css_color.cpp.i

CMakeFiles/flower.dir/source/css/css_color.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/flower.dir/source/css/css_color.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/miguelrodovia/dev/flower/source/css/css_color.cpp -o CMakeFiles/flower.dir/source/css/css_color.cpp.s

CMakeFiles/flower.dir/source/css/css_selector.cpp.o: CMakeFiles/flower.dir/flags.make
CMakeFiles/flower.dir/source/css/css_selector.cpp.o: /home/miguelrodovia/dev/flower/source/css/css_selector.cpp
CMakeFiles/flower.dir/source/css/css_selector.cpp.o: CMakeFiles/flower.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/flower.dir/source/css/css_selector.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/flower.dir/source/css/css_selector.cpp.o -MF CMakeFiles/flower.dir/source/css/css_selector.cpp.o.d -o CMakeFiles/flower.dir/source/css/css_selector.cpp.o -c /home/miguelrodovia/dev/flower/source/css/css_selector.cpp

CMakeFiles/flower.dir/source/css/css_selector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/flower.dir/source/css/css_selector.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/miguelrodovia/dev/flower/source/css/css_selector.cpp > CMakeFiles/flower.dir/source/css/css_selector.cpp.i

CMakeFiles/flower.dir/source/css/css_selector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/flower.dir/source/css/css_selector.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/miguelrodovia/dev/flower/source/css/css_selector.cpp -o CMakeFiles/flower.dir/source/css/css_selector.cpp.s

# Object files for target flower
flower_OBJECTS = \
"CMakeFiles/flower.dir/source/main.cpp.o" \
"CMakeFiles/flower.dir/source/html.cpp.o" \
"CMakeFiles/flower.dir/source/css/css.cpp.o" \
"CMakeFiles/flower.dir/source/css/css_color.cpp.o" \
"CMakeFiles/flower.dir/source/css/css_selector.cpp.o"

# External object files for target flower
flower_EXTERNAL_OBJECTS =

flower: CMakeFiles/flower.dir/source/main.cpp.o
flower: CMakeFiles/flower.dir/source/html.cpp.o
flower: CMakeFiles/flower.dir/source/css/css.cpp.o
flower: CMakeFiles/flower.dir/source/css/css_color.cpp.o
flower: CMakeFiles/flower.dir/source/css/css_selector.cpp.o
flower: CMakeFiles/flower.dir/build.make
flower: http/libhttp.a
flower: draw/libdraw.a
flower: windowcreator/libwc.a
flower: /usr/lib/libgnutls.so
flower: /usr/lib/libX11.so
flower: /usr/lib/libcairo.so
flower: /usr/lib/libpangocairo-1.0.so
flower: /usr/lib/libpango-1.0.so
flower: /usr/lib/libgobject-2.0.so
flower: /usr/lib/libglib-2.0.so
flower: /usr/lib/libharfbuzz.so
flower: /usr/lib/libcairo.so
flower: CMakeFiles/flower.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/miguelrodovia/dev/flower/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable flower"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/flower.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/flower.dir/build: flower
.PHONY : CMakeFiles/flower.dir/build

CMakeFiles/flower.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/flower.dir/cmake_clean.cmake
.PHONY : CMakeFiles/flower.dir/clean

CMakeFiles/flower.dir/depend:
	cd /home/miguelrodovia/dev/flower/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/miguelrodovia/dev/flower /home/miguelrodovia/dev/flower /home/miguelrodovia/dev/flower/build /home/miguelrodovia/dev/flower/build /home/miguelrodovia/dev/flower/build/CMakeFiles/flower.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/flower.dir/depend
