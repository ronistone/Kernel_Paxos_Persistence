//
// Created by roni on 13/09/19.
//
#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"
#include "read_test_device_operations.h"
#include "read_persistence_device_operations.h"
#include <linux/wait.h>
#include <linux/kthread.h>

static paxos_kernel_device readTestDevice;
static struct task_struct **responseThread;
static int messagesReceived = 0, messagesFound = 0, current_thread = 0;
static int N_THREADS = 100;

int read_test_open(struct inode *inodep, struct file *filep) {
    LOG_DEBUG( "Mutex Address %p", &(readTestDevice.char_mutex));
    if (!mutex_trylock(&(readTestDevice.char_mutex))) {
        printk(KERN_ALERT "Device char: Device used by another process");
        return -EBUSY;
    }
    messagesFound = messagesReceived = 0;
    return 0;
}

// returns 0 if it has to stop, >0 when it reads something, and <0 on error
ssize_t read_test_read(struct file *filep, char *buffer, size_t len,
                  loff_t *offset) {
  if (readTestDevice.working == 0)
    return -1;

  return len;
}

int wait_response(void* param){
  kernel_device_callback* callback = (kernel_device_callback*)param;
  printk("Enter in block!\n");
  int wait_response = wait_event_timeout(callback -> response_wait, callback -> response != NULL, 2000);
  if(wait_response == 0){
      printk("Wait Response: Timeout and condition is false\n");
  } else  {
      printk("Wait Response: Condition is true with reponse=%d\n", wait_response);
  }
  printk("End the block!\n");
  if(callback != NULL && callback -> response != NULL && callback -> response -> value.paxos_value_len > 0) {
    printk("Paxos_accepted [%d] -> {%d} = %s\n", callback -> response->iid,
           callback -> response->value.paxos_value_len, callback -> response -> value.paxos_value_val);
    messagesFound++;
  } else {
    printk("Paxos_accepted [%d] -> no Message\n", callback -> response->iid);
  }
  do_exit(0);
  return 0;
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
    kernel_device_callback* callback = vmalloc(sizeof(kernel_device_callback));
    init_waitqueue_head(&(callback -> response_wait));
    callback->response = NULL;
    int error = read_persistence_add_message(buffer, len, callback);
    if(error) {
      printk("Buffer full!\n");
      return len;
    }
//    if(responseThread[current_thread] != NULL){
//        kthread_stop(responseThread[current_thread]);
//        responseThread[current_thread] = NULL;
//    }
    responseThread[current_thread] = kthread_run(wait_response, (void*) callback, "responseThread");
    current_thread = (current_thread + 1) % N_THREADS;
//    wait_response((void*) (&callback));
    messagesReceived++;

    return len;
}

unsigned int read_test_poll(struct file *file, poll_table *wait) {
//    poll_wait(file, &(readTestDevice.access_wait), wait);
//    if (atomic_read(&(readTestDevice.used_buf)) > 0)
//        return POLLIN;

    return 0;
}

int read_test_release(struct inode *inodep, struct file *filep) {
    mutex_unlock(&(readTestDevice.char_mutex));

    printk(KERN_INFO "\n\n===================================\n\n");
    printk(KERN_INFO "Messages Received: %d\n", messagesReceived);
    printk(KERN_INFO "Messages Found on Persistence: %d\n", messagesFound);
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

    responseThread = vmalloc(sizeof(struct task_struct*) * N_THREADS);

    return &readTestDevice;
}