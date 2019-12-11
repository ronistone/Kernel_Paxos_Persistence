//
// Created by roni on 06/10/19.
//

#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"
#include "paxos_types.h"


paxos_kernel_device writePersistenceDevice;


int write_persistence_open(struct inode *inodep, struct file *filep) {
    LOG_DEBUG( "Mutex Address %p", &(writePersistenceDevice.char_mutex));
    if (!mutex_trylock(&(writePersistenceDevice.char_mutex))) {
        printk(KERN_ALERT "Write Persistence Device char: Device used by another process");
        return -EBUSY;
    }
    return 0;
}

// returns 0 if it has to stop, >0 when it reads something, and <0 on error
ssize_t write_persistence_read(struct file *filep, char *buffer, size_t len,
                      loff_t *offset) {
    int error_count;
    size_t llen;

    if (!writePersistenceDevice.working)
        return 0;

    llen = sizeof(paxos_accepted) + writePersistenceDevice.msg_buf[writePersistenceDevice.first_buf]->value.paxos_value_len;
    error_count = copy_to_user(buffer, (char *)(writePersistenceDevice.msg_buf[writePersistenceDevice.first_buf]), llen);
    atomic_dec(&(writePersistenceDevice.used_buf));

//    LOG_INFO("Write Persistence Device char: read %zu bytes!", llen);

    if (error_count != 0) {
        paxerr("send fewer characters to the user");
        return -1;
    } else {
      writePersistenceDevice.first_buf = (writePersistenceDevice.first_buf + 1) % BUFFER_SIZE;
    }
//    int i;
//    printk("Reading message bytes=[%zu]  ---> ", llen);
//    for(i=0;i< llen;i++)
//      printk("%d", buffer[i]);
//    printk("\n");
    return llen;
}

ssize_t write_persistence_write(struct file *filep, const char *buffer, size_t len,
                       loff_t *offset) {
  if (writePersistenceDevice.working == 0)
    return -1;

  return len;
}

unsigned int write_persistence_poll(struct file *file, poll_table *wait) {
    poll_wait(file, &(writePersistenceDevice.access_wait), wait);
    if (atomic_read(&(writePersistenceDevice.used_buf)) > 0)
        return POLLIN;

    return 0;
}

int write_persistence_release(struct inode *inodep, struct file *filep) {
    mutex_unlock(&(writePersistenceDevice.char_mutex));
    // LOG_INFO("Messages left %d", atomic_read(&used_buf));
    atomic_set(&(writePersistenceDevice.used_buf), 0);
    writePersistenceDevice.current_buf = 0;
    writePersistenceDevice.first_buf = 0;
    printk(KERN_INFO "Write Persistence Device char: Device successfully closed");
    return 0;
}

void write_persistence_add_message(const char* msg, size_t size) {
  if (atomic_read(&(writePersistenceDevice.used_buf)) >= BUFFER_SIZE) {
    if (printk_ratelimit())
      printk(KERN_INFO "Write Persistence Buffer is full! Lost a value");
    return;
  }

//  writePersistenceDevice.msg_buf[writePersistenceDevice.current_buf]->size = size;
//  memcpy(writePersistenceDevice.msg_buf[writePersistenceDevice.current_buf]->value, msg, size);
//  writePersistenceDevice.msg_buf[writePersistenceDevice.current_buf]->size = size;
  memcpy(writePersistenceDevice.msg_buf[writePersistenceDevice.current_buf], msg, size);
  writePersistenceDevice.current_buf = (writePersistenceDevice.current_buf + 1) % BUFFER_SIZE;
  atomic_inc(&(writePersistenceDevice.used_buf));
  wake_up_interruptible(&(writePersistenceDevice.access_wait));
}

paxos_kernel_device* createWritePersistenceDevice(void) {

    writePersistenceDevice.msg_buf = NULL;
    writePersistenceDevice.charClass = NULL;
    writePersistenceDevice.charDevice = NULL;

    writePersistenceDevice.fops.owner = THIS_MODULE;
    writePersistenceDevice.fops.open = write_persistence_open;
    writePersistenceDevice.fops.read = write_persistence_read;
    writePersistenceDevice.fops.write = write_persistence_write;
    writePersistenceDevice.fops.release = write_persistence_release;
    writePersistenceDevice.fops.poll = write_persistence_poll;

    return &writePersistenceDevice;
}