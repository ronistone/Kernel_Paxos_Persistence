//
// Created by roni on 13/09/19.
//
#include "kernel_device.h"
#include "kernel_client.h"
#include "common.h"
#include "read_test_device_operations.h"
#include "read_persistence_device_operations.h"
#include "workers_pool.h"
#include <linux/wait.h>
#include <linux/kthread.h>

static paxos_kernel_device readTestDevice;
static workers_pool *pool;
static int messagesReceived = 0, messagesFound = 0;
static int N_THREADS = 100;

static persistence_work* createPersistenceWork();

int read_test_open(struct inode *inodep, struct file *filep) {
    LOG_DEBUG("Mutex Address %p", &(readTestDevice.char_mutex));
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

void wait_response(struct kthread_work *param) {
    persistence_work* work = container_of(param, persistence_work, work);
    kernel_device_callback *callback = work -> param;
    char* threadName = param -> worker -> task -> comm;

//    printk("[%s  ->  %ld] Enter in block!\n", threadName, param -> worker -> task -> state);
    int wait_response = wait_event_timeout(callback->response_wait, callback->response != NULL, 10000);
    if (wait_response == 0) {
        printk("[%s  ->  %ld] Wait Response: Timeout and condition is false\n", threadName, param -> worker -> task -> state);
    } else {
        printk("[%s  ->  %ld] Wait Response: Condition is true with reponse=%d\n", threadName, param -> worker -> task -> state, wait_response);
    }
//    printk("[%s  ->  %ld] End the block!\n", threadName, param -> worker -> task -> state);
    if (callback != NULL && callback->response != NULL && callback->response->value.paxos_value_len > 0) {
//        printk("[%s  ->  %ld] Paxos_accepted [%d] -> {%d} = %s\n", threadName, param -> worker -> task -> state, callback->response->iid,
//               callback->response->value.paxos_value_len, callback->response->value.paxos_value_val);
        messagesFound++;
    } else {
//        printk("[%s  -> %ld] Paxos_accepted [%d] -> no Message\n", threadName, param -> worker -> task -> state, callback->buffer_id);
    }
}

ssize_t read_test_write(struct file *filep, const char *buffer, size_t len,
                        loff_t *offset) {
    if (readTestDevice.working == 0)
        return -1;

    persistence_work *persistenceWork = createPersistenceWork();

    if(persistenceWork == NULL){
        printk("ERROR to create persistence work\n");
        return len;
    }
    int error = read_persistence_add_message(buffer, len, persistenceWork->param);
    if (error) {
        printk("Buffer full!\n");
        return len;
    }

    add_work(pool, persistenceWork);
    messagesReceived++;

    return len;
}

static persistence_work* createPersistenceWork() {
    kernel_device_callback *callback = vmalloc(sizeof(kernel_device_callback));
    persistence_work *persistenceWork = vmalloc(sizeof(persistence_work));

    if( callback != NULL && persistenceWork != NULL ) {
        init_waitqueue_head(&(callback->response_wait));
        callback->response = NULL;

        persistenceWork->param = callback;
        init_kthread_work(&(persistenceWork->work), wait_response);
    }

    return persistenceWork;
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

paxos_kernel_device *createReadTestDevice(void) {

    readTestDevice.msg_buf = NULL;
    readTestDevice.charClass = NULL;
    readTestDevice.charDevice = NULL;

    readTestDevice.fops.owner = THIS_MODULE;
    readTestDevice.fops.open = read_test_open;
    readTestDevice.fops.read = read_test_read;
    readTestDevice.fops.write = read_test_write;
    readTestDevice.fops.release = read_test_release;
    readTestDevice.fops.poll = read_test_poll;

    pool = create_pool(N_THREADS);
    if (pool == NULL) {
        printk("ERROR: Thread Pool was not created\n");
    }

    return &readTestDevice;
}