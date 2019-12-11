//
// Created by roni on 13/09/19.
//
#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"
#include "write_persistence_device_operations.h"

paxos_kernel_device kleanerDevice;


int klearner_open(struct inode *inodep, struct file *filep) {
    LOG_DEBUG( "Mutex Address %p", &(kleanerDevice.char_mutex));
    if (!mutex_trylock(&(kleanerDevice.char_mutex))) {
        printk(KERN_ALERT "Device char: Device used by another process");
        return -EBUSY;
    }
    return 0;
}

// returns 0 if it has to stop, >0 when it reads something, and <0 on error
ssize_t klearner_read(struct file *filep, char *buffer, size_t len,
                  loff_t *offset) {
  if (kleanerDevice.working == 0)
    return -1;

  return len;
}

ssize_t klearner_write(struct file *filep, const char *buffer, size_t len,
                   loff_t *offset) {
    if (kleanerDevice.working == 0)
        return -1;

//    printk(KERN_INFO "Writing the message %zx: --> ", len);
//    int i;
//    for(i=0;i< len;i++)
//      printk("%d", buffer[i]);
//    printk("\n");
    write_persistence_add_message(buffer, len);

    return len;
}

unsigned int klearner_poll(struct file *file, poll_table *wait) {
    poll_wait(file, &(kleanerDevice.access_wait), wait);
    if (atomic_read(&(kleanerDevice.used_buf)) > 0)
        return POLLIN;

    return 0;
}

int klearner_release(struct inode *inodep, struct file *filep) {
    mutex_unlock(&(kleanerDevice.char_mutex));
    // LOG_INFO("Messages left %d", atomic_read(&used_buf));
    atomic_set(&(kleanerDevice.used_buf), 1);
    printk(KERN_INFO "Device Char: Device successfully closed");
    return 0;
}

paxos_kernel_device* createKLeanerDevice(void) {

    kleanerDevice.msg_buf = NULL;
    kleanerDevice.charClass = NULL;
    kleanerDevice.charDevice = NULL;

    kleanerDevice.fops.owner = THIS_MODULE;
    kleanerDevice.fops.open = klearner_open;
    kleanerDevice.fops.read = klearner_read;
    kleanerDevice.fops.write = klearner_write;
    kleanerDevice.fops.release = klearner_release;
    kleanerDevice.fops.poll = klearner_poll;

    return &kleanerDevice;
}