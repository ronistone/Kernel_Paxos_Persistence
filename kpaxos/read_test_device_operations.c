//
// Created by roni on 13/09/19.
//
#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"
#include "read_test_device_operations.h"
#include "read_persistence_device_operations.h"
#include <linux/wait.h>

paxos_kernel_device readTestDevice;
long messagesReceived = 0, messagesFound = 0;

int read_test_open(struct inode *inodep, struct file *filep) {
    messagesFound = messagesReceived = 0;
    LOG_DEBUG( "Mutex Address %p", &(readTestDevice.char_mutex));
    if (!mutex_trylock(&(readTestDevice.char_mutex))) {
        printk(KERN_ALERT "Device char: Device used by another process");
        return -EBUSY;
    }
    return 0;
}

// returns 0 if it has to stop, >0 when it reads something, and <0 on error
ssize_t read_test_read(struct file *filep, char *buffer, size_t len,
                  loff_t *offset) {
  if (readTestDevice.working == 0)
    return -1;

  return len;
}

ssize_t read_test_write(struct file *filep, const char *buffer, size_t len,
                   loff_t *offset) {
    if (readTestDevice.working == 0)
        return -1;
//    int i;
//    printk("READ_TEST[ %zu ]: %d", len, ((paxos_accepted*)buffer)->iid );
//    for(i=0;i<len;i++){
//        printk("%c", buffer[i]);
//    }
//    printk("\n");
    kernel_device_callback callback;
    init_waitqueue_head(&(callback.response_wait));
    callback.response = NULL;
    read_persistence_add_message(buffer, len, &callback);
    printk("Enter in block!\n");
    wait_event_timeout(callback.response_wait, callback.response == NULL, 1);
//    wait_event(callback.response_wait, callback.response == NULL);
    printk("End the block!\n");
    if(callback.response -> value.paxos_value_len > 0) {
      printk("Paxos_accepted [%d] -> {%d} = %s\n", callback.response->iid,
             callback.response->value.paxos_value_len, callback.response -> value.paxos_value_val);
      messagesFound++;
    } else {
      printk("Paxos_accepted [%d] -> no Message\n", callback.response->iid);
    }
    messagesReceived++;

    return len;
}

unsigned int read_test_poll(struct file *file, poll_table *wait) {
    poll_wait(file, &(readTestDevice.access_wait), wait);
    if (atomic_read(&(readTestDevice.used_buf)) > 0)
        return POLLIN;

    return 0;
}

int read_test_release(struct inode *inodep, struct file *filep) {
    mutex_unlock(&(readTestDevice.char_mutex));
    // LOG_INFO("Messages left %d", atomic_read(&used_buf));
    atomic_set(&(readTestDevice.used_buf), 1);
    printk(KERN_INFO "\n\n===================================\n\n");
    printk(KERN_INFO "Messages Received: %ld\n", messagesReceived);
    printk(KERN_INFO "Messages Found on Persistence: %ld\n", messagesFound);
    printk(KERN_INFO "\n\n===================================\n\n");
    printk(KERN_INFO "Device Char: Device successfully closed");
    return 0;
}

paxos_kernel_device* createReadTestDevice(void) {

    readTestDevice.msg_buf = NULL;
    readTestDevice.charClass = NULL;
    readTestDevice.charDevice = NULL;

    readTestDevice.fops.owner = THIS_MODULE;
    readTestDevice.fops.open = read_test_open;
    readTestDevice.fops.read = read_test_read;
    readTestDevice.fops.write = read_test_write;
    readTestDevice.fops.release = read_test_release;
    readTestDevice.fops.poll = read_test_poll;

    return &readTestDevice;
}