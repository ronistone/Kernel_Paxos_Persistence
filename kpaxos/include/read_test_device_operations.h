//
// Created by roni on 13/09/19.
//

#ifndef KERNEL_PAXOS_PERSISTENCE_READ_TEST_DEVICE_OPERATIONS_H
#define KERNEL_PAXOS_PERSISTENCE_READ_TEST_DEVICE_OPERATIONS_H

extern paxos_kernel_device*  createReadTestDevice(void);
extern void         persistence_device_destroy(paxos_kernel_device* kernel_device);

#endif //KERNEL_PAXOS_PERSISTENCE_READ_TEST_DEVICE_OPERATIONS_H
