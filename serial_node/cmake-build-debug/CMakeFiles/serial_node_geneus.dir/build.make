# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /home/zhuzhengming/software/clion-2021.2.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/zhuzhengming/software/clion-2021.2.3/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zhuzhengming/workspace/src/serial_node

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhuzhengming/workspace/src/serial_node/cmake-build-debug

# Utility rule file for serial_node_geneus.

# Include any custom commands dependencies for this target.
include CMakeFiles/serial_node_geneus.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/serial_node_geneus.dir/progress.make

serial_node_geneus: CMakeFiles/serial_node_geneus.dir/build.make
.PHONY : serial_node_geneus

# Rule to build all files generated by this target.
CMakeFiles/serial_node_geneus.dir/build: serial_node_geneus
.PHONY : CMakeFiles/serial_node_geneus.dir/build

CMakeFiles/serial_node_geneus.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/serial_node_geneus.dir/cmake_clean.cmake
.PHONY : CMakeFiles/serial_node_geneus.dir/clean

CMakeFiles/serial_node_geneus.dir/depend:
	cd /home/zhuzhengming/workspace/src/serial_node/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhuzhengming/workspace/src/serial_node /home/zhuzhengming/workspace/src/serial_node /home/zhuzhengming/workspace/src/serial_node/cmake-build-debug /home/zhuzhengming/workspace/src/serial_node/cmake-build-debug /home/zhuzhengming/workspace/src/serial_node/cmake-build-debug/CMakeFiles/serial_node_geneus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/serial_node_geneus.dir/depend

