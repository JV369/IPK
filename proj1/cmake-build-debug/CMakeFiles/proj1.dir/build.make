# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/jan/clion-2016.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/jan/clion-2016.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jan/IPK/proj1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jan/IPK/proj1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/proj1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/proj1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/proj1.dir/flags.make

CMakeFiles/proj1.dir/client.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/client.c.o: ../client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan/IPK/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/proj1.dir/client.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/client.c.o   -c /home/jan/IPK/proj1/client.c

CMakeFiles/proj1.dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/client.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/IPK/proj1/client.c > CMakeFiles/proj1.dir/client.c.i

CMakeFiles/proj1.dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/client.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/IPK/proj1/client.c -o CMakeFiles/proj1.dir/client.c.s

CMakeFiles/proj1.dir/client.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/client.c.o.requires

CMakeFiles/proj1.dir/client.c.o.provides: CMakeFiles/proj1.dir/client.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/client.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/client.c.o.provides

CMakeFiles/proj1.dir/client.c.o.provides.build: CMakeFiles/proj1.dir/client.c.o


CMakeFiles/proj1.dir/server.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/server.c.o: ../server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan/IPK/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/proj1.dir/server.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/server.c.o   -c /home/jan/IPK/proj1/server.c

CMakeFiles/proj1.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/server.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/IPK/proj1/server.c > CMakeFiles/proj1.dir/server.c.i

CMakeFiles/proj1.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/server.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/IPK/proj1/server.c -o CMakeFiles/proj1.dir/server.c.s

CMakeFiles/proj1.dir/server.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/server.c.o.requires

CMakeFiles/proj1.dir/server.c.o.provides: CMakeFiles/proj1.dir/server.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/server.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/server.c.o.provides

CMakeFiles/proj1.dir/server.c.o.provides.build: CMakeFiles/proj1.dir/server.c.o


CMakeFiles/proj1.dir/test.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/test.c.o: ../test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan/IPK/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/proj1.dir/test.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/test.c.o   -c /home/jan/IPK/proj1/test.c

CMakeFiles/proj1.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/test.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jan/IPK/proj1/test.c > CMakeFiles/proj1.dir/test.c.i

CMakeFiles/proj1.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/test.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jan/IPK/proj1/test.c -o CMakeFiles/proj1.dir/test.c.s

CMakeFiles/proj1.dir/test.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/test.c.o.requires

CMakeFiles/proj1.dir/test.c.o.provides: CMakeFiles/proj1.dir/test.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/test.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/test.c.o.provides

CMakeFiles/proj1.dir/test.c.o.provides.build: CMakeFiles/proj1.dir/test.c.o


# Object files for target proj1
proj1_OBJECTS = \
"CMakeFiles/proj1.dir/client.c.o" \
"CMakeFiles/proj1.dir/server.c.o" \
"CMakeFiles/proj1.dir/test.c.o"

# External object files for target proj1
proj1_EXTERNAL_OBJECTS =

proj1: CMakeFiles/proj1.dir/client.c.o
proj1: CMakeFiles/proj1.dir/server.c.o
proj1: CMakeFiles/proj1.dir/test.c.o
proj1: CMakeFiles/proj1.dir/build.make
proj1: CMakeFiles/proj1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jan/IPK/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable proj1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proj1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/proj1.dir/build: proj1

.PHONY : CMakeFiles/proj1.dir/build

CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/client.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/server.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/test.c.o.requires

.PHONY : CMakeFiles/proj1.dir/requires

CMakeFiles/proj1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/proj1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/proj1.dir/clean

CMakeFiles/proj1.dir/depend:
	cd /home/jan/IPK/proj1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jan/IPK/proj1 /home/jan/IPK/proj1 /home/jan/IPK/proj1/cmake-build-debug /home/jan/IPK/proj1/cmake-build-debug /home/jan/IPK/proj1/cmake-build-debug/CMakeFiles/proj1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/proj1.dir/depend

