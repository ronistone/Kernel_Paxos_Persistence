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

static struct mutex char_mutex;
// static struct mutex buffer_mutex;

// The device-driver class struct pointer
static struct class *charClass = NULL;
// The device-driver device struct pointer
static struct device *charDevice = NULL;
static wait_queue_head_t access_wait;
static char *de_name, *clas_name;
static int majorNumber, working;
static struct user_msg *msg_buf = NULL;
static atomic_t used_buf;

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = kdev_open,
    .read = kdev_read,
    .write = kdev_write,
    .release = kdev_release,
    .poll = kdev_poll,
};

static void paxerr(char *err) {
  printk(KERN_ALERT "Device Char: failed to %s", err);
  // working = 0;
}

int kdev_open(struct inode *inodep, struct file *filep) {
  if (!mutex_trylock(&char_mutex)) {
    printk(KERN_ALERT "Device char: Device used by another process");
    return -EBUSY;
  }
  return 0;
}

// returns 0 if it has to stop, >0 when it reads something, and <0 on error
ssize_t kdev_read(struct file *filep, char *buffer, size_t len,
                  loff_t *offset) {
  int error_count;
  size_t llen;

  if (!working)
    return 0;

  atomic_dec(&used_buf);
  llen = sizeof(struct user_msg) + msg_buf->size;
  error_count = copy_to_user(buffer, (char *)msg_buf, llen);

  atomic_inc(&used_buf);
  if (error_count != 0) {
    paxerr("send fewer characters to the user");
    return -1;
  }

  return llen;
}

ssize_t kdev_write(struct file *filep, const char *buffer, size_t len,
                   loff_t *offset) {
  if (working == 0)
    return -1;

  struct user_msg *tmp = (struct user_msg *)buffer;
  if (tmp->size > 0 && tmp->size <= MSG_LEN) {
    msg_buf->size = tmp->size;
    memcpy(msg_buf->value, tmp->value, tmp->size);
  } else {
    len = 0;
  }

  return len;
}

unsigned int kdev_poll(struct file *file, poll_table *wait) {
  poll_wait(file, &access_wait, wait);
  if (atomic_read(&used_buf) > 0)
    return POLLIN;

  return 0;
}

int kdev_release(struct inode *inodep, struct file *filep) {
  mutex_unlock(&char_mutex);
  // LOG_INFO("Messages left %d", atomic_read(&used_buf));
  atomic_set(&used_buf, 1);
  printk(KERN_INFO "Device Char: Device successfully closed");
  return 0;
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

static int major_number(int id, char *name) {
  allocate_name_folder(&de_name, name, id);
  majorNumber = register_chrdev(0, de_name, &fops);
  if (majorNumber < 0) {
    paxerr("register major number");
    return majorNumber;
  }
  printk(KERN_INFO "Device Char: Device Char %s registered correctly with major"
                   " number %d",
         de_name, majorNumber);
  return 0;
}

static int reg_dev_class(int id, char *name) {
  allocate_name(&clas_name, name, id);
  charClass = class_create(THIS_MODULE, clas_name);
  if (IS_ERR(charClass)) {
    unregister_chrdev(majorNumber, de_name);
    paxerr("register device class");
    return -1;
  }
  printk(KERN_INFO "Device Char: device class %s registered correctly",
         clas_name);
  return 0;
}

static int reg_char_class(void) {
  charDevice =
      device_create(charClass, NULL, MKDEV(majorNumber, 0), NULL, de_name);
  if (IS_ERR(charDevice)) {
    class_destroy(charClass);
    unregister_chrdev(majorNumber, de_name);
    paxerr("create the device");
    return -1;
  }
  return 0;
}

int kdevchar_init(int id, char *name) {
  if (name == NULL)
    return 0;
  printk(KERN_INFO "Initializing the Device Char");
  working = 1;

  // Register major number
  if (major_number(id, name) < 0)
    return -1;

  // Register the device class
  if (reg_dev_class(id, name) < 0)
    return -1;

  // Register the device driver
  if (reg_char_class() < 0)
    return -1;

  msg_buf = vmalloc(sizeof(struct user_msg) + MSG_LEN);
  msg_buf->size = MSG_LEN;
  strcpy(msg_buf->value, "HI there! If you see this message, you've read "
                         "from a char device");

  mutex_init(&char_mutex);
  init_waitqueue_head(&access_wait);
  atomic_set(&used_buf, 1);
  printk(KERN_INFO "Device class created correctly");
  return 0;
}

void kdevchar_exit(void) {
  working = 0;
  atomic_inc(&used_buf);
  wake_up_interruptible(&access_wait);
  mutex_destroy(&char_mutex);
  device_destroy(charClass, MKDEV(majorNumber, 0)); // remove the device
  class_unregister(charClass); // unregister the device class
  class_destroy(charClass);    // remove the device class

  if (msg_buf)
    vfree(msg_buf);

  pfree(de_name);
  pfree(clas_name);
  unregister_chrdev(majorNumber, de_name); // unregister the major number
  printk(KERN_INFO "Unloaded\n");
}
