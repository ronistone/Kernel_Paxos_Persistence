//
// Created by roni on 12/02/2020.
//

#include "workers_pool.h"


void __add_work(struct kthread_worker* worker, persistence_work* work){

    // add Worker to queue


}


void add_work(workers_pool* pool, persistence_work* work){

    if( pool != NULL ) {
        __add_work( pool -> workers[pool -> current_worker], work );
        pool -> current_worker = (pool -> current_worker + 1) % pool -> num_workers;
    } else {
        printk("ERROR: workers pool is NULL!\n");
    }

}
