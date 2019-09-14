#include "kernel_device.h"
#include "common.h"
#include "kernel_client.h"
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>

#define BUFFER_SIZE 100000


void paxerr(char *err) {
  printk(KERN_ALERT "Device Char: failed to %s", err);
  // working = 0;
}

static void allocate_name(char **dest, char *name, int id) {
  size_t len = strlen(name) + 1;
  *dest = pmalloc(len + 1);
  memcpy(*dest, name, len);
  (*dest)[len - 1] = id + '0';
  (*dest)[len] = '\0';
}

static void allocate_name_folder(char **dest, char *name, int id) {
  char *folder = "paxos";
  size_t f_len = strlen(folder);
  size_t len = strlen(name);
  *dest = pmalloc(f_len + len + 3);
  memcpy(*dest, folder, f_len);
  (*dest)[f_len] = '/';
  (*dest)[f_len + 1] = '\0';
  strcat(*dest, name);
  (*dest)[f_len + len + 1] = id + '0';
  (*dest)[f_len + len + 2] = '\0';
}

static int major_number(int id, char *name, paxos_kernel_device* kernelDevice) {
  allocate_name_folder(&(kernelDevice -> de_name), name, id);
  kernelDevice -> majorNumber = register_chrdev(0, kernelDevice -> de_name, &(kernelDevice -> fops));
  if (kernelDevice -> majorNumber < 0) {
    paxerr("register major number");
    return kernelDevice -> majorNumber;
  }
  printk(KERN_INFO "Device Char: Device Char %s registered correctly with major"
                   " number %d",
         kernelDevice -> de_name, kernelDevice -> majorNumber);
  return 0;
}

static int reg_dev_class(int id, char *name, paxos_kernel_device* kernelDevice) {
  allocate_name(&(kernelDevice -> clas_name), name, id);
  kernelDevice -> charClass = class_create(THIS_MODULE, kernelDevice -> clas_name);
  if (IS_ERR(kernelDevice -> charClass)) {
    unregister_chrdev(kernelDevice -> majorNumber, kernelDevice -> de_name);
    paxerr("register device class");
    return -1;
  }
  printk(KERN_INFO "Device Char: device class %s registered correctly",
         kernelDevice -> clas_name);
  return 0;
}

static int reg_char_class(paxos_kernel_device* kernelDevice) {
  kernelDevice -> charDevice =
      device_create(
          kernelDevice -> charClass,
          NULL,
          MKDEV( kernelDevice -> majorNumber, kernelDevice -> minorNumber),
          NULL,
          kernelDevice -> de_name
      );

  if (IS_ERR(kernelDevice -> charDevice)) {
    class_destroy(kernelDevice -> charClass);
    unregister_chrdev(kernelDevice -> majorNumber, kernelDevice -> de_name);
    paxerr("create the device");
    return -1;
  }
  return 0;
}

int kdevchar_init(int id, char* name, paxos_kernel_device* kernel_device) {
  if (name == NULL || kernel_device == NULL)
    return 0;
  printk(KERN_INFO "Initializing the Device Char");
  kernel_device -> working = 1;

  // Register major number
  if (major_number(id, name, kernel_device) < 0)
    return -1;

  // Register the device class
  if (reg_dev_class(id, name, kernel_device) < 0)
    return -1;

  // Register the device driver
  if (reg_char_class(kernel_device) < 0)
    return -1;

  kernel_device -> msg_buf = vmalloc(sizeof(struct user_msg) + MSG_LEN);
  kernel_device -> msg_buf->size = MSG_LEN;
  strcpy(kernel_device -> msg_buf->value, "HI there! If you see this message, you've read "
                         "from a char device");

  mutex_init(& (kernel_device -> char_mutex));
  init_waitqueue_head(&(kernel_device -> access_wait));
  atomic_set(&(kernel_device -> used_buf), 1);
  printk(KERN_INFO "Device class created correctly");
  return 0;
}

void kdevchar_exit(paxos_kernel_device* kernel_device) {
  kernel_device -> working = 0;
  atomic_inc(&(kernel_device -> used_buf));
  wake_up_interruptible(&(kernel_device -> access_wait));
  mutex_destroy(&(kernel_device -> char_mutex));
  device_destroy(kernel_device -> charClass, MKDEV(kernel_device -> majorNumber, kernel_device -> minorNumber)); // remove the device
  class_unregister(kernel_device -> charClass); // unregister the device class
  class_destroy(kernel_device -> charClass);    // remove the device class

  if (kernel_device -> msg_buf)
    vfree(kernel_device -> msg_buf);

  pfree(kernel_device -> de_name);
  pfree(kernel_device -> clas_name);
  unregister_chrdev(kernel_device -> majorNumber, kernel_device -> de_name); // unregister the major number
  printk(KERN_INFO "Unloaded\n");
}
