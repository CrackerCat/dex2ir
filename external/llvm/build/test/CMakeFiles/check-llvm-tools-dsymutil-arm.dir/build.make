# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/m/AOSP5/external/llvm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/m/AOSP5/external/llvm/build

# Utility rule file for check-llvm-tools-dsymutil-arm.

# Include the progress variables for this target.
include test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/progress.make

test/CMakeFiles/check-llvm-tools-dsymutil-arm:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/m/AOSP5/external/llvm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running lit suite /home/m/AOSP5/external/llvm/test/tools/dsymutil/ARM"
	cd /home/m/AOSP5/external/llvm/build/test && /usr/bin/python2.7 /home/m/AOSP5/external/llvm/utils/lit/lit.py -sv --param llvm_site_config=/home/m/AOSP5/external/llvm/build/test/lit.site.cfg --param llvm_unit_site_config=/home/m/AOSP5/external/llvm/build/test/Unit/lit.site.cfg /home/m/AOSP5/external/llvm/test/tools/dsymutil/ARM

check-llvm-tools-dsymutil-arm: test/CMakeFiles/check-llvm-tools-dsymutil-arm
check-llvm-tools-dsymutil-arm: test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/build.make

.PHONY : check-llvm-tools-dsymutil-arm

# Rule to build all files generated by this target.
test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/build: check-llvm-tools-dsymutil-arm

.PHONY : test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/build

test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/clean:
	cd /home/m/AOSP5/external/llvm/build/test && $(CMAKE_COMMAND) -P CMakeFiles/check-llvm-tools-dsymutil-arm.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/clean

test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/depend:
	cd /home/m/AOSP5/external/llvm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/m/AOSP5/external/llvm /home/m/AOSP5/external/llvm/test /home/m/AOSP5/external/llvm/build /home/m/AOSP5/external/llvm/build/test /home/m/AOSP5/external/llvm/build/test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/check-llvm-tools-dsymutil-arm.dir/depend
