//
// Created by roni on 13/09/19.
//
#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"


paxos_kernel_device kleanerDevice;


int klearner_open(struct inode *inodep, struct file *filep) {
    if (!mutex_trylock(&(kleanerDevice.char_mutex))) {
        printk(KERN_ALERT "Device char: Device used by another process");
        return -EBUSY;
    }
    return 0;
}

// returns 0 if it has to stop, >0 when it reads something, and <0 on error
ssize_t klearner_read(struct file *filep, char *buffer, size_t len,
                  loff_t *offset) {
    int error_count;
    size_t llen;

    if (!kleanerDevice.working)
        return 0;

    atomic_dec(&(kleanerDevice.used_buf));
    llen = sizeof(struct user_msg) + (kleanerDevice.msg_buf->size);
    error_count = copy_to_user(buffer, (char *)(kleanerDevice.msg_buf), llen);

    LOG_INFO("Device char: read %zu bytes!", llen);

    atomic_inc(&(kleanerDevice.used_buf));
    if (error_count != 0) {
        paxerr("send fewer characters to the user");
        return -1;
    }

    return llen;
}

ssize_t klearner_write(struct file *filep, const char *buffer, size_t len,
                   loff_t *offset) {
    if (kleanerDevice.working == 0)
        return -1;

    struct user_msg *tmp = (struct user_msg *)buffer;
    if (tmp->size > 0 && tmp->size <= MSG_LEN) {
        kleanerDevice.msg_buf->size = tmp->size;
        memcpy(kleanerDevice.msg_buf->value, tmp->value, tmp->size);
    } else {
        len = 0;
    }

    LOG_INFO("Device char: write %zu bytes!", len);
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

paxos_kernel_device createKLeanerDevice(void) {

    kleanerDevice.msg_buf = NULL;
    kleanerDevice.charClass = NULL;
    kleanerDevice.charDevice = NULL;
    kleanerDevice.minorNumber = 1;

    kleanerDevice.fops.owner = THIS_MODULE;
    kleanerDevice.fops.open = klearner_open;
    kleanerDevice.fops.read = klearner_read;
    kleanerDevice.fops.write = klearner_write;
    kleanerDevice.fops.release = klearner_release;
    kleanerDevice.fops.poll = klearner_poll;

    return kleanerDevice;
}