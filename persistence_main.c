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

#define DEVICE_NAME "paxoschar"
#define CLASS_NAME "paxos"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ronistone Junior");
MODULE_DESCRIPTION("Module to persist data from the kernel");
MODULE_VERSION("0.1");

static int id = 0;

static int __init paxos_persistence_init(void) {
  printk(KERN_INFO "PAXOS PERSISTENCE: Initializing paxos persistence");
  printk(KERN_INFO "");

  kdevchar_init(id, "persistence");

  printk(KERN_INFO "PAXOS PERSISTENCE: paxos persistence initialized");
  printk(KERN_INFO "");
  return 0;
}

static void __exit paxos_persistence_exit(void) {
  kdevchar_exit();
  printk(KERN_INFO "PAXOS PERSISTENCE: goodbye");
  printk(KERN_INFO "");
}

module_init(paxos_persistence_init);
module_exit(paxos_persistence_exit);