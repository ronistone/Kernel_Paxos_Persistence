//
// Created by roni on 13/09/19.
//

#ifndef KERNEL_PAXOS_PERSISTENCE_READ_PERSISTENCE_DEVICE_OPERATIONS_H
#define KERNEL_PAXOS_PERSISTENCE_READ_PERSISTENCE_DEVICE_OPERATIONS_H

extern int                  read_persistence_open(struct inode*, struct file*);
extern int                  read_persistence_release(struct inode*, struct file*);
extern ssize_t              read_persistence_read(struct file*, char*, size_t, loff_t*);
extern ssize_t              read_persistence_write(struct file*, const char*, size_t, loff_t*);
extern unsigned int         read_persistence_poll(struct file*, poll_table* wait);
extern paxos_kernel_device* createReadPersistenceDevice(void);
extern int                  read_persistence_add_message(const char* msg, size_t size, kernel_device_callback* callback);

#endif //KERNEL_PAXOS_PERSISTENCE_READ_PERSISTENCE_DEVICE_OPERATIONS_H
