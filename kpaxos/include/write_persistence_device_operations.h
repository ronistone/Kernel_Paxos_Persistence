//
// Created by roni on 13/09/19.
//

#ifndef KERNEL_PAXOS_PERSISTENCE_WRITE_PERSISTENCE_DEVICE_OPERATIONS_H
#define KERNEL_PAXOS_PERSISTENCE_WRITE_PERSISTENCE_DEVICE_OPERATIONS_H

extern paxos_kernel_device*     createWritePersistenceDevice(void);
extern kernel_device_callback*  write_persistence_add_message(const char* msg, size_t size);

#endif //KERNEL_PAXOS_PERSISTENCE_WRITE_PERSISTENCE_DEVICE_OPERATIONS_H
