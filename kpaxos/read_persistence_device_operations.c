//
// Created by roni on 07/10/19.
//

#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"
#include "paxos_types.h"


paxos_kernel_device readPersistenceDevice_;


int read_persistence_open(struct inode *inodep, struct file *filep) {
  LOG_DEBUG( "Mutex Address %p", &(readPersistenceDevice_.char_mutex));
  if (!mutex_trylock(&(readPersistenceDevice_.char_mutex))) {
    printk(KERN_ALERT "Read Persistence Device char: Device used by another process");
    return -EBUSY;
  }
  return 0;
}

// returns 0 if it has to stop, >0 when it reads something, and <0 on error
ssize_t read_persistence_read(struct file *filep, char *buffer, size_t len,
                               loff_t *offset) {
  int error_count;
  size_t llen;

  if (!readPersistenceDevice_.working)
    return 0;

//    llen = (readPersistenceDevice_.msg_buf[readPersistenceDevice_.first_buf]->size);
  llen = sizeof(paxos_accepted);
  error_count = copy_to_user(buffer, (char *)(readPersistenceDevice_.msg_buf[readPersistenceDevice_.first_buf]), llen);
  atomic_dec(&(readPersistenceDevice_.used_buf));

  LOG_INFO("Read Persistence Device char: read %zu bytes!", llen);

  if (error_count != 0) {
    paxerr("send fewer characters to the user");
    return -1;
  } else {
    readPersistenceDevice_.first_buf = (readPersistenceDevice_.first_buf + 1) % BUFFER_SIZE;
  }
  int i;
  printk("Reading message bytes=[%zu]  ---> ", llen);
  for(i=0;i< llen;i++)
    printk("%d", buffer[i]);
  printk("\n");
  return llen;
}

ssize_t read_persistence_write(struct file *filep, const char *buffer, size_t len,
                                loff_t *offset) {
  if (readPersistenceDevice_.working == 0)
    return -1;

  return len;
}

unsigned int read_persistence_poll(struct file *file, poll_table *wait) {
  poll_wait(file, &(readPersistenceDevice_.access_wait), wait);
  if (atomic_read(&(readPersistenceDevice_.used_buf)) > 0)
    return POLLIN;

  return 0;
}

int read_persistence_release(struct inode *inodep, struct file *filep) {
  mutex_unlock(&(readPersistenceDevice_.char_mutex));
  // LOG_INFO("Messages left %d", atomic_read(&used_buf));
  atomic_set(&(readPersistenceDevice_.used_buf), 0);
  readPersistenceDevice_.current_buf = 0;
  readPersistenceDevice_.first_buf = 0;
  printk(KERN_INFO "Read Persistence Device char: Device successfully closed");
  return 0;
}

void read_persistence_add_message(const char* msg, size_t size) {
  if (atomic_read(&(readPersistenceDevice_.used_buf)) >= BUFFER_SIZE) {
    if (printk_ratelimit())
      printk(KERN_INFO "Read Persistence Buffer is full! Lost a value");
  }

//  readPersistenceDevice_.msg_buf[readPersistenceDevice_.current_buf]->size = size;
//  memcpy(readPersistenceDevice_.msg_buf[readPersistenceDevice_.current_buf]->value, msg, size);
//  readPersistenceDevice_.msg_buf[readPersistenceDevice_.current_buf]->size = size;
  memcpy(readPersistenceDevice_.msg_buf[readPersistenceDevice_.current_buf], msg, size);
  readPersistenceDevice_.current_buf = (readPersistenceDevice_.current_buf + 1) % BUFFER_SIZE;
  atomic_inc(&(readPersistenceDevice_.used_buf));
  wake_up_interruptible(&(readPersistenceDevice_.access_wait));
}


paxos_kernel_device* createReadPersistenceDevice(void) {

  readPersistenceDevice_.msg_buf = NULL;
  readPersistenceDevice_.charClass = NULL;
  readPersistenceDevice_.charDevice = NULL;

  readPersistenceDevice_.fops.owner = THIS_MODULE;
  readPersistenceDevice_.fops.open = read_persistence_open;
  readPersistenceDevice_.fops.read = read_persistence_read;
  readPersistenceDevice_.fops.write = read_persistence_write;
  readPersistenceDevice_.fops.release = read_persistence_release;
  readPersistenceDevice_.fops.poll = read_persistence_poll;

  return &readPersistenceDevice_;
}