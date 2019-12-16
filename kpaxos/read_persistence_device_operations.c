//
// Created by roni on 07/10/19.
//

#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"
#include "paxos_types.h"
#include "paxos.h"


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
  int error_count_accepted, error_count_buffer_id;
  size_t llen;

  if (!readPersistenceDevice_.working)
    return 0;

  llen = sizeof(paxos_accepted) + readPersistenceDevice_.msg_buf[readPersistenceDevice_.first_buf]->value.paxos_value_len;
  error_count_buffer_id = copy_to_user(buffer, (char *)(&readPersistenceDevice_.first_buf), sizeof(int));
  error_count_accepted = copy_to_user(&buffer[sizeof(int)], (char *)(readPersistenceDevice_.msg_buf[readPersistenceDevice_.first_buf]), llen);
  llen += sizeof(int);


  LOG_INFO("Read Persistence Device char: read %zu bytes!", llen);

  if (error_count_accepted != 0 || error_count_buffer_id != 0) {
    paxerr("send fewer characters to the user");
    return -1;
  } else {
    readPersistenceDevice_.first_buf = (readPersistenceDevice_.first_buf + 1) % BUFFER_SIZE;
  }
  return llen;
}

ssize_t read_persistence_write(struct file *filep, const char *buffer, size_t len,
                                loff_t *offset) {
  if (readPersistenceDevice_.working == 0)
    return -1;

  int error_count_accepted, error_count_buffer_id, error_count_value = 0;
  kernel_device_message* message = vmalloc(sizeof(kernel_device_message));
  paxos_accepted* accepted = vmalloc(sizeof(paxos_accepted));

  error_count_buffer_id = copy_from_user(message, buffer, sizeof(int));
  error_count_accepted  = copy_from_user(accepted, &buffer[sizeof(int)], sizeof(paxos_accepted));

  if (error_count_accepted != 0 || error_count_buffer_id != 0) {
    paxerr("receive fewer characters from the user");
    return -1;
  }

  if(accepted -> value.paxos_value_len > 0) {
    accepted -> value.paxos_value_val = vmalloc(accepted -> value.paxos_value_len);
    error_count_value = copy_from_user(accepted -> value.paxos_value_val, &buffer[sizeof(int) + sizeof(paxos_accepted)], accepted -> value.paxos_value_len);
  }

  if(error_count_value != 0){
    paxerr("receive fewer paxos value characters from the user");
    return -1;
  }

  readPersistenceDevice_.callback_buf[message -> buffer_id] -> response = &accepted;
  wake_up_interruptible(readPersistenceDevice_.callback_buf[message -> buffer_id] -> response_wait);

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
  atomic_set(&(readPersistenceDevice_.used_buf), 0);
  readPersistenceDevice_.current_buf = 0;
  readPersistenceDevice_.first_buf = 0;
  printk(KERN_INFO "Read Persistence Device char: Device successfully closed");
  return 0;
}

int read_persistence_add_message(const char* msg, size_t size, kernel_device_callback* callback) {
  if (atomic_read(&(readPersistenceDevice_.used_buf)) >= BUFFER_SIZE) {
    if (printk_ratelimit())
      printk(KERN_INFO "Read Persistence Buffer is full! Lost a value");
    return -1;
  }
  atomic_inc(&(readPersistenceDevice_.used_buf));

  // BEGIN Save paxos accepted
  memcpy(readPersistenceDevice_.msg_buf[readPersistenceDevice_.current_buf], msg, sizeof(paxos_accepted));

  if(size > sizeof(paxos_accepted) + MAX_PAXOS_VALUE_SIZE){
    printk(KERN_ERR "Data will be truncated.");
    size = sizeof(paxos_accepted) + MAX_PAXOS_VALUE_SIZE;
    readPersistenceDevice_.msg_buf[readPersistenceDevice_.current_buf] -> value.paxos_value_len = MAX_PAXOS_VALUE_SIZE - sizeof(paxos_accepted);
  }

  paxos_accepted* accepted = readPersistenceDevice_.msg_buf[readPersistenceDevice_.current_buf];
  if(accepted -> value.paxos_value_len > 0){
    accepted -> value.paxos_value_val = vmalloc(accepted -> value.paxos_value_len);
    memcpy(accepted -> value.paxos_value_val, &msg[sizeof(paxos_accepted)], accepted -> value.paxos_value_len);
  }
  // END Save paxos accepted

  readPersistenceDevice_.callback_buf[readPersistenceDevice_.current_buf] = callback;

  readPersistenceDevice_.current_buf = (readPersistenceDevice_.current_buf + 1) % BUFFER_SIZE;

  wake_up_interruptible(&(readPersistenceDevice_.access_wait));
  return 0;
}


paxos_kernel_device* createReadPersistenceDevice(void) {

  readPersistenceDevice_.msg_buf = NULL;
  readPersistenceDevice_.charClass = NULL;
  readPersistenceDevice_.charDevice = NULL;
  readPersistenceDevice_.callback_buf = NULL;

  readPersistenceDevice_.fops.owner = THIS_MODULE;
  readPersistenceDevice_.fops.open = read_persistence_open;
  readPersistenceDevice_.fops.read = read_persistence_read;
  readPersistenceDevice_.fops.write = read_persistence_write;
  readPersistenceDevice_.fops.release = read_persistence_release;
  readPersistenceDevice_.fops.poll = read_persistence_poll;

  return &readPersistenceDevice_;
}