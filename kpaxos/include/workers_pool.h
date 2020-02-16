//
// Created by roni on 12/02/2020.
//

#ifndef KERNEL_PAXOS_PERSISTENCE_WORKERS_POOL_H
#define KERNEL_PAXOS_PERSISTENCE_WORKERS_POOL_H

#include <linux/kthread.h>
#include "kernel_device.h"

typedef struct workers_pool {

    struct kthread_worker** worker;
    struct task_struct **workerThread;
    int current_worker;
    int num_workers;

} workers_pool;

typedef struct persistence_work {
    struct kthread_work work;
    kernel_device_callback* param;
} persistence_work;

#endif //KERNEL_PAXOS_PERSISTENCE_WORKERS_POOL_H
