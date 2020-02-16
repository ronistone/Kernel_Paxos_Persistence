//
// Created by roni on 12/02/2020.
//

#include "workers_pool.h"


void __add_work(struct kthread_worker* worker, persistence_work* work){

    // add Worker to queue


}

static workers_pool* create_pool(int num_workers){

    workers_pool* pool = vmalloc(sizeof(workers_pool));
    int i;

    if(pool == NULL) {
        printk("ERROR: Fail to alloc workers_pool!");
    }

    pool -> worker = vmalloc(sizeof(kthread_worker*) * num_workers);

    if(pool -> worker == NULL) {
        printk("ERROR: Fail to alloc %d workers from pool!", num_workers);
    }

    for (i = 0; i < num_workers; i++ ) {
        pool -> worker[i] = vmalloc(sizeof(kthread_worker));

        if(pool -> worker[i] == NULL) {
            printk("ERROR: Fail to alloc worker[%d] on the pool!", i);
        }

        kthread_init_worker(pool -> worker[i]);
    }

    pool -> num_workers = num_workers;
    pool -> current_worker = 0;


}

void add_work(workers_pool* pool, persistence_work* work){

    if( pool != NULL ) {
        __add_work( pool -> workers[pool -> current_worker], work );
        pool -> current_worker = (pool -> current_worker + 1) % pool -> num_workers;
    } else {
        printk("ERROR: workers pool is NULL!\n");
    }

}
