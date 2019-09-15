//
// Created by roni on 13/09/19.
//

#ifndef KERNEL_PAXOS_PERSISTENCE_KLEANER_DEVICE_OPERATIONS_H
#define KERNEL_PAXOS_PERSISTENCE_KLEANER_DEVICE_OPERATIONS_H

extern int                  kdev_open(struct inode*, struct file*);
extern int                  kdev_release(struct inode*, struct file*);
extern ssize_t              kdev_read(struct file*, char*, size_t, loff_t*);
extern ssize_t              kdev_write(struct file*, const char*, size_t, loff_t*);
extern unsigned int         kdev_poll(struct file*, poll_table* wait);
extern paxos_kernel_device*  createKLeanerDevice(void);

#endif //KERNEL_PAXOS_PERSISTENCE_KLEANER_DEVICE_OPERATIONS_H
