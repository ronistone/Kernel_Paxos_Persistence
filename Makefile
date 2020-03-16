obj-m+= \
	persistence.o

persistence-y:= \
    paxos/paxos.o \
    kpaxos/workers_pool.o \
	kpaxos/write_persistence_device_operations.o \
	kpaxos/read_persistence_device_operations.o \
	kpaxos/write_test_device_operations.o \
	kpaxos/read_test_device_operations.o \
	kpaxos/kernel_device.o \
	persistence_main.o \

KDIR ?= /lib/modules/$(shell uname -r)/build
BUILD_DIR ?= $(PWD)/build
BUILD_DIR_MAKEFILE ?= $(PWD)/build/Makefile

C_COMP:= -std=c99
G_COMP:= -std=gnu99
USR_FLAGS:= -Wall -D user_space
USRC_OBJS := $(BUILD_DIR)/user_chardev.o
USRSTOR_OBJS := $(BUILD_DIR)/user_storage.o
LMDBOP_OBJS := $(BUILD_DIR)/lmdb_operations.o

EXTRA_CFLAGS:= -I$(PWD)/kpaxos/include -I$(PWD)/paxos/include -I$(HOME)/local/include
EXTRALMDB_FLAG:= -llmdb
EXTRASTORE_FLAG:= -lpthread
DEBUG_FLAGS += -g -DDEBUG
ccflags-y:= $(G_COMP) -Wall -Wno-declaration-after-statement -Wframe-larger-than=3100 -O3


all: $(BUILD_DIR) kernel_app user_chardev user_storage teste_kernel_device

#	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
#	$(CC) testekerneldevice.c -o test
kernel_app: $(BUILD_DIR_MAKEFILE)
	make -C $(KDIR) M=$(BUILD_DIR) src=$(PWD) modules


$(BUILD_DIR):
	mkdir -p "$@/paxos"
	mkdir -p "$@/kpaxos"

$(BUILD_DIR_MAKEFILE): $(BUILD_DIR)
	touch "$@"

$(BUILD_DIR)/%.o: kpaxos/%.c
	$(CC) $(G_COMP) $(USR_FLAGS) $(EXTRA_CFLAGS) -c $< -o $@

user_chardev: $(USRC_OBJS)
	$(CC) $(USR_FLAGS) $(EXTRA_CFLAGS) -o $(BUILD_DIR)/$@ $^

user_storage: $(LMDBOP_OBJS) $(USRSTOR_OBJS)
	$(CC) $(USR_FLAGS) $(EXTRA_CFLAGS) $(EXTRASTORE_FLAG) $(EXTRALMDB_FLAG) -o $(BUILD_DIR)/$@ $^

user_storage_debug: $(LMDBOP_OBJS) $(USRSTOR_OBJS)
	$(CC) $(DEBUG_FLAGS) $(USR_FLAGS) $(EXTRA_CFLAGS) $(EXTRASTORE_FLAG) $(EXTRALMDB_FLAG) -o $(BUILD_DIR)/$@ $^

clean:
	make -C $(KDIR) M=$(BUILD_DIR) src=$(PWD) clean
	-rm -rf build

debug: $(BUILD_DIR) debug_command user_chardev user_storage_debug teste_kernel_device_debug

debug_command: $(BUILD_DIR_MAKEFILE)
	make -C $(KDIR) M=$(BUILD_DIR) src=$(PWD) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS) $(DEBUG_FLAGS)"

#teste_kernel_device:
#	gcc testekerneldevice.c -o build/testes -Ipaxos/include -Ikpaxos/
teste_kernel_device:
	gcc testekerneldevice.c -o build/testes -Ipaxos/include -Ikpaxos/include -lpthread

teste_kernel_device_debug:
	gcc -g testekerneldevice.c -o build/testes -Ipaxos/include -Ikpaxos/include -lpthread