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
#include "kleaner_device_operations.h"
#include "write_persistence_device_operations.h"

#define DEVICE_NAME "paxoschar"
#define CLASS_NAME "paxos"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ronistone Junior");
MODULE_DESCRIPTION("Module to persist data from the kernel");
MODULE_VERSION("0.1");

static int id = 0, writePersistenceId = 1;
const char* MOD_NAME = "Persistence";

static paxos_kernel_device *klearnerDevice = NULL, *writePersistence = NULL;

static int __init paxos_persistence_init(void) {
  printk(KERN_INFO "PAXOS PERSISTENCE: Initializing paxos persistence");
  printk(KERN_INFO "");

  klearnerDevice = createKLeanerDevice();
  kdevchar_init(id, "persistence", klearnerDevice);

  writePersistence = createWritePersistenceDevice();
  kdevchar_init(writePersistenceId, "write-persistence", writePersistence);


  printk(KERN_INFO "PAXOS PERSISTENCE: paxos persistence initialized");
  printk(KERN_INFO "");
  return 0;
}

static void __exit paxos_persistence_exit(void) {
  kdevchar_exit(klearnerDevice);
  kdevchar_exit(writePersistence);
  printk(KERN_INFO "PAXOS PERSISTENCE: goodbye");
  printk(KERN_INFO "");
}

module_init(paxos_persistence_init);
module_exit(paxos_persistence_exit);