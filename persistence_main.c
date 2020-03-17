//
// Created by roni on 31/07/19.
//

#include <linux/device.h> // Header to support the kernel Driver Model
#include <linux/fs.h>     // Header for the Linux file system support
#include <linux/init.h>   // Macros used to mark up functions e.g. __init __exit
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/uaccess.h>

#include "kernel_device.h"
#include "write_test_device_operations.h"
#include "read_test_device_operations.h"
#include "write_persistence_device_operations.h"
#include "read_persistence_device_operations.h"

#define DEVICE_NAME "paxoschar"
#define CLASS_NAME "paxos"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ronistone Junior");
MODULE_DESCRIPTION("Module to persist data from the kernel");
MODULE_VERSION("0.1");

static int writeTestId = 0, readTestId = 1, writePersistenceId = 2, readPersistenceId = 3;
const char* MOD_NAME = "Persistence";

static paxos_kernel_device *writeTestDevice = NULL, *readTestDevice = NULL, *writePersistence = NULL, *readPersistence = NULL;

static int __init paxos_persistence_init(void) {
  printk(KERN_INFO "PAXOS PERSISTENCE: Initializing paxos persistence");
  printk(KERN_INFO "");

  writeTestDevice = createWriteTestDevice();
  kdevchar_init(writeTestId, "write-test", writeTestDevice);

  readTestDevice  = createReadTestDevice();
  kdevchar_init(readTestId, "read-test", readTestDevice);

  writePersistence = createWritePersistenceDevice();
  kdevchar_init(writePersistenceId, "write-persistence", writePersistence);

  readPersistence = createReadPersistenceDevice();
  kdevchar_init(readPersistenceId, "read-persistence", readPersistence);


  printk(KERN_INFO "PAXOS PERSISTENCE: paxos persistence initialized");
  printk(KERN_INFO "");
  return 0;
}

static void __exit paxos_persistence_exit(void) {
  kdevchar_exit(writeTestDevice);
  persistence_device_destroy(writeTestDevice);

  kdevchar_exit(readTestDevice);
  persistence_device_destroy(readTestDevice);

  kdevchar_exit(writePersistence);
  kdevchar_exit(readPersistence);
  printk(KERN_INFO "PAXOS PERSISTENCE: goodbye");
  printk(KERN_INFO "");
}

module_init(paxos_persistence_init);
module_exit(paxos_persistence_exit);