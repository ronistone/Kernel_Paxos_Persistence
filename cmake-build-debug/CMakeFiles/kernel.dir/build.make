# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /opt/clion-2019.3.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2019.3.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/kernel.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kernel.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kernel.dir/flags.make

CMakeFiles/kernel.dir/kpaxos/kernel_device.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/kernel_device.c.o: ../kpaxos/kernel_device.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/kernel.dir/kpaxos/kernel_device.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.dir/kpaxos/kernel_device.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/kernel_device.c

CMakeFiles/kernel.dir/kpaxos/kernel_device.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/kernel_device.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/kernel_device.c > CMakeFiles/kernel.dir/kpaxos/kernel_device.c.i

CMakeFiles/kernel.dir/kpaxos/kernel_device.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/kernel_device.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/kernel_device.c -o CMakeFiles/kernel.dir/kpaxos/kernel_device.c.s

CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.o: ../kpaxos/read_persistence_device_operations.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/read_persistence_device_operations.c

CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/read_persistence_device_operations.c > CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.i

CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/read_persistence_device_operations.c -o CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.s

CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.o: ../kpaxos/write_test_device_operations.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/write_test_device_operations.c

CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/write_test_device_operations.c > CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.i

CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/write_test_device_operations.c -o CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.s

CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.o: ../kpaxos/lmdb_operations.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -o CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/lmdb_operations.c

CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/lmdb_operations.c > CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.i

CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/lmdb_operations.c -o CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.s

CMakeFiles/kernel.dir/kpaxos/workers_pool.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/workers_pool.c.o: ../kpaxos/workers_pool.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/kernel.dir/kpaxos/workers_pool.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.dir/kpaxos/workers_pool.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/workers_pool.c

CMakeFiles/kernel.dir/kpaxos/workers_pool.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/workers_pool.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/workers_pool.c > CMakeFiles/kernel.dir/kpaxos/workers_pool.c.i

CMakeFiles/kernel.dir/kpaxos/workers_pool.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/workers_pool.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/workers_pool.c -o CMakeFiles/kernel.dir/kpaxos/workers_pool.c.s

CMakeFiles/kernel.dir/kpaxos/user_chardev.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/user_chardev.c.o: ../kpaxos/user_chardev.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/kernel.dir/kpaxos/user_chardev.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -o CMakeFiles/kernel.dir/kpaxos/user_chardev.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/user_chardev.c

CMakeFiles/kernel.dir/kpaxos/user_chardev.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/user_chardev.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/user_chardev.c > CMakeFiles/kernel.dir/kpaxos/user_chardev.c.i

CMakeFiles/kernel.dir/kpaxos/user_chardev.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/user_chardev.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/user_chardev.c -o CMakeFiles/kernel.dir/kpaxos/user_chardev.c.s

CMakeFiles/kernel.dir/persistence_main.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/persistence_main.c.o: ../persistence_main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/kernel.dir/persistence_main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.dir/persistence_main.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/persistence_main.c

CMakeFiles/kernel.dir/persistence_main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/persistence_main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/persistence_main.c > CMakeFiles/kernel.dir/persistence_main.c.i

CMakeFiles/kernel.dir/persistence_main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/persistence_main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/persistence_main.c -o CMakeFiles/kernel.dir/persistence_main.c.s

CMakeFiles/kernel.dir/build/persistence.mod.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/build/persistence.mod.c.o: ../build/persistence.mod.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/kernel.dir/build/persistence.mod.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.dir/build/persistence.mod.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/build/persistence.mod.c

CMakeFiles/kernel.dir/build/persistence.mod.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/build/persistence.mod.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/build/persistence.mod.c > CMakeFiles/kernel.dir/build/persistence.mod.c.i

CMakeFiles/kernel.dir/build/persistence.mod.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/build/persistence.mod.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/build/persistence.mod.c -o CMakeFiles/kernel.dir/build/persistence.mod.c.s

CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.o: ../kpaxos/write_persistence_device_operations.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/write_persistence_device_operations.c

CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/write_persistence_device_operations.c > CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.i

CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/write_persistence_device_operations.c -o CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.s

CMakeFiles/kernel.dir/kpaxos/user_storage.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/user_storage.c.o: ../kpaxos/user_storage.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/kernel.dir/kpaxos/user_storage.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -o CMakeFiles/kernel.dir/kpaxos/user_storage.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/user_storage.c

CMakeFiles/kernel.dir/kpaxos/user_storage.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/user_storage.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/user_storage.c > CMakeFiles/kernel.dir/kpaxos/user_storage.c.i

CMakeFiles/kernel.dir/kpaxos/user_storage.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/user_storage.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -std=gnu99 -Wall -D user_space -I..//vagrant/kpaxos/include -I..//vagrant/paxos/include -I..//home/vagrant/local/include -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/user_storage.c -o CMakeFiles/kernel.dir/kpaxos/user_storage.c.s

CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.o: ../kpaxos/read_test_device_operations.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/read_test_device_operations.c

CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/read_test_device_operations.c > CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.i

CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/kpaxos/read_test_device_operations.c -o CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.s

CMakeFiles/kernel.dir/testekerneldevice.c.o: CMakeFiles/kernel.dir/flags.make
CMakeFiles/kernel.dir/testekerneldevice.c.o: ../testekerneldevice.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/kernel.dir/testekerneldevice.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -I../paxos/include -I../kpaxos/include -o CMakeFiles/kernel.dir/testekerneldevice.c.o   -c /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/testekerneldevice.c

CMakeFiles/kernel.dir/testekerneldevice.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.dir/testekerneldevice.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -I../paxos/include -I../kpaxos/include -E /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/testekerneldevice.c > CMakeFiles/kernel.dir/testekerneldevice.c.i

CMakeFiles/kernel.dir/testekerneldevice.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.dir/testekerneldevice.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -c -I../paxos/include -I../kpaxos/include -S /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/testekerneldevice.c -o CMakeFiles/kernel.dir/testekerneldevice.c.s

kernel: CMakeFiles/kernel.dir/kpaxos/kernel_device.c.o
kernel: CMakeFiles/kernel.dir/kpaxos/read_persistence_device_operations.c.o
kernel: CMakeFiles/kernel.dir/kpaxos/write_test_device_operations.c.o
kernel: CMakeFiles/kernel.dir/kpaxos/lmdb_operations.c.o
kernel: CMakeFiles/kernel.dir/kpaxos/workers_pool.c.o
kernel: CMakeFiles/kernel.dir/kpaxos/user_chardev.c.o
kernel: CMakeFiles/kernel.dir/persistence_main.c.o
kernel: CMakeFiles/kernel.dir/build/persistence.mod.c.o
kernel: CMakeFiles/kernel.dir/kpaxos/write_persistence_device_operations.c.o
kernel: CMakeFiles/kernel.dir/kpaxos/user_storage.c.o
kernel: CMakeFiles/kernel.dir/kpaxos/read_test_device_operations.c.o
kernel: CMakeFiles/kernel.dir/testekerneldevice.c.o
kernel: CMakeFiles/kernel.dir/build.make

.PHONY : kernel

# Rule to build all files generated by this target.
CMakeFiles/kernel.dir/build: kernel

.PHONY : CMakeFiles/kernel.dir/build

CMakeFiles/kernel.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kernel.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kernel.dir/clean

CMakeFiles/kernel.dir/depend:
	cd /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug /home/roni/Workspace/ufu/TCC/Kernel_Paxos_Persistence/cmake-build-debug/CMakeFiles/kernel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kernel.dir/depend
