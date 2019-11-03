//
// Created by roni on 03/11/19.
//

#ifndef KERNEL_PAXOS_PERSISTENCE_LMDB_OPERATIONS_H
#define KERNEL_PAXOS_PERSISTENCE_LMDB_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "paxos.h"
#include "paxos_types.h"

#include <lmdb.h>

struct lmdb_storage
{
    MDB_env* env;
    MDB_txn* txn;
    MDB_dbi dbi;
    int acceptor_id;
};

extern int      lmdb_storage_open(void* handle);
extern int      lmdb_storage_tx_begin(void* handle);
extern int      lmdb_storage_tx_commit(void* handle);
extern void     lmdb_storage_tx_abort(void* handle);
extern void     lmdb_storage_close(void* handle);
extern int      lmdb_storage_put(void* handle, paxos_accepted* acc);
extern int      lmdb_storage_get(void* handle, iid_t iid, paxos_accepted* out);

extern void     buffer_to_paxos_accepted(char* buffer, paxos_accepted* out);
extern char*    paxos_accepted_to_buffer(paxos_accepted* acc);


#endif //KERNEL_PAXOS_PERSISTENCE_LMDB_OPERATIONS_H
