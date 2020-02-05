//
// Created by roni on 13/09/19.
//
#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"
#include "write_test_device_operations.h"
#include "write_persistence_device_operations.h"

static paxos_kernel_device writeTestDevice;


int write_test_open(struct inode *inodep, struct file *filep) {
    LOG_DEBUG( "Mutex Address %p", &(writeTestDevice.char_mutex));
    if (!mutex_trylock(&(writeTestDevice.char_mutex))) {
        printk(KERN_ALERT "Device char: Device used by another process");
        return -EBUSY;
    }
    return 0;
}

// returns 0 if it has to stop, >0 when it reads something, and <0 on error
ssize_t write_test_read(struct file *filep, char *buffer, size_t len,
                  loff_t *offset) {
  if (writeTestDevice.working == 0)
    return -1;

  return len;
}

ssize_t write_test_write(struct file *filep, const char *buffer, size_t len,
                   loff_t *offset) {
    if (writeTestDevice.working == 0)
        return -1;

    write_persistence_add_message(buffer, len, NULL);

    return len;
}

unsigned int write_test_poll(struct file *file, poll_table *wait) {
    poll_wait(file, &(writeTestDevice.access_wait), wait);
    if (atomic_read(&(writeTestDevice.used_buf)) > 0)
        return POLLIN;

    return 0;
}

int write_test_release(struct inode *inodep, struct file *filep) {
    mutex_unlock(&(writeTestDevice.char_mutex));
    atomic_set(&(writeTestDevice.used_buf), 1);
    printk(KERN_INFO "Device Char: Device successfully closed");
    return 0;
}

paxos_kernel_device* createWriteTestDevice(void) {

    writeTestDevice.msg_buf = NULL;
    writeTestDevice.charClass = NULL;
    writeTestDevice.charDevice = NULL;

    writeTestDevice.fops.owner = THIS_MODULE;
    writeTestDevice.fops.open = write_test_open;
    writeTestDevice.fops.read = write_test_read;
    writeTestDevice.fops.write = write_test_write;
    writeTestDevice.fops.release = write_test_release;
    writeTestDevice.fops.poll = write_test_poll;

    return &writeTestDevice;
}