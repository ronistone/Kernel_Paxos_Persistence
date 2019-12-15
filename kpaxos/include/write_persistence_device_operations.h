//
// Created by roni on 13/09/19.
//

#ifndef KERNEL_PAXOS_PERSISTENCE_WRITE_PERSISTENCE_DEVICE_OPERATIONS_H
#define KERNEL_PAXOS_PERSISTENCE_WRITE_PERSISTENCE_DEVICE_OPERATIONS_H

extern int                  write_persistence_open(struct inode*, struct file*);
extern int                  write_persistence_release(struct inode*, struct file*);
extern ssize_t              write_persistence_read(struct file*, char*, size_t, loff_t*);
extern ssize_t              write_persistence_write(struct file*, const char*, size_t, loff_t*);
extern unsigned int         write_persistence_poll(struct file*, poll_table* wait);
extern paxos_kernel_device* createWritePersistenceDevice(void);
extern int                  write_persistence_add_message(const char* msg, size_t size, kernel_device_callback* callback);

#endif //KERNEL_PAXOS_PERSISTENCE_WRITE_PERSISTENCE_DEVICE_OPERATIONS_H
