//
// Created by roni on 03/11/19.
//

#include "lmdb_operations.h"

#define DEFAULT_SIZE 10*1024*1024

static const char* db_env_path = "/tmp/paxos";
static size_t lmdb_mapsize = DEFAULT_SIZE;

void
buffer_to_paxos_accepted(char* buffer, paxos_accepted* out)
{
  memcpy(out, buffer, sizeof(paxos_accepted));
  if (out->value.paxos_value_len > 0) {
    out->value.paxos_value_val = malloc(out->value.paxos_value_len);
    memcpy(out->value.paxos_value_val,
           &buffer[sizeof(paxos_accepted)],
           out->value.paxos_value_len);
  }
}

char*
paxos_accepted_to_buffer(paxos_accepted* acc)
{
  size_t len = acc->value.paxos_value_len;
  char* buffer = malloc(sizeof(paxos_accepted) + len);
  if (buffer == NULL)
    return NULL;
  memcpy(buffer, acc, sizeof(paxos_accepted));
  if (len > 0) {
    memcpy(&buffer[sizeof(paxos_accepted)], acc->value.paxos_value_val, len);
  }
  return buffer;
}

static int
lmdb_compare_iid(const MDB_val* lhs, const MDB_val* rhs)
{
  iid_t lid, rid;
  assert(lhs->mv_size == sizeof(iid_t));
  assert(rhs->mv_size == sizeof(iid_t));
  lid = *((iid_t*) lhs->mv_data);
  rid = *((iid_t*) rhs->mv_data);
  return (lid == rid) ? 0 : (lid < rid) ? -1 : 1;
}

void
lmdb_storage_close(void* handle)
{
  struct lmdb_storage* s = handle;
  if (s->txn) {
    mdb_txn_abort(s->txn);
  }
  if (s->dbi) {
    mdb_close(s->env, s->dbi);
  }
  if (s->env) {
    mdb_env_close(s->env);
  }
//  free(s);
  printf("lmdb storage closed successfully\n");
}

//static struct lmdb_storage*
//lmdb_storage_new(int acceptor_id)
//{
//  struct lmdb_storage* s = malloc(sizeof(struct lmdb_storage));
//  memset(s, 0, sizeof(struct lmdb_storage));
//  s->acceptor_id = acceptor_id;
//  return s;
//}

static int
lmdb_storage_init(struct lmdb_storage* s, char* db_env_path)
{
  int result;
  struct stat sb;
  MDB_env* env = NULL;
  MDB_txn* txn = NULL;
  MDB_dbi dbi = 0;

  int dir_exists = (stat(db_env_path, &sb) == 0);

  if (!dir_exists && (mkdir(db_env_path, S_IRWXU) != 0)) {
    printf("Failed to create env dir %s", db_env_path);
    result = -1;
    goto error;
  }

  if ((result = mdb_env_create(&env)) != 0) {
    printf("Could not create lmdb environment. %s\n", mdb_strerror(result));
    goto error;
  }
  if ((result = mdb_env_set_mapsize(env, lmdb_mapsize)) != 0) {
    printf("Could not set lmdb map size. %s\n", mdb_strerror(result));
    goto error;
  }

  if ((result = mdb_env_open(env, db_env_path, 0, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) != 0) {
    printf("%d\n", result);
    printf("Could not open lmdb environment at %s. %s\n", db_env_path, mdb_strerror(result));
    goto error;
  }
  if ((result = mdb_txn_begin(env, NULL, 0, &txn)) != 0) {
    printf("Could not start txn on lmdb environment at %s. %s\n", db_env_path, mdb_strerror(result));
    goto error;
  }
  if ((result = mdb_open(txn, NULL, MDB_INTEGERKEY, &dbi)) != 0) {
    printf("Could not open db on lmdb environment at %s. %s\n", db_env_path, mdb_strerror(result));
    goto error;
  }
  if ((result = mdb_set_compare(txn, dbi, lmdb_compare_iid)) != 0) {
    printf("Could setup compare function on lmdb environment at %s. %s\n", db_env_path, mdb_strerror(result));
    goto error;
  }
  if ((result = mdb_txn_commit(txn)) != 0) {
    printf("Could commit txn on lmdb environment at %s. %s\n", db_env_path, mdb_strerror(result));
    goto error;
  }

  s->env = env;
  s->dbi = dbi;

  return 0;
  error:
  if (txn) {
    mdb_txn_abort(txn);
  }
  if (dbi) {
    mdb_close(env, dbi);
  }
  if (env) {
    mdb_env_close(env);
  }
  return -1;
}

int
lmdb_storage_open(void* handle)
{
  struct lmdb_storage* s = handle;
  char* lmdb_env_path = NULL;
  struct stat sb;
  int dir_exists, result;
  size_t lmdb_env_path_length = strlen(db_env_path) + 16;

  lmdb_env_path = malloc(lmdb_env_path_length);
  snprintf(lmdb_env_path, lmdb_env_path_length, "%s_%d",
           db_env_path, s->acceptor_id);

  dir_exists = (stat(lmdb_env_path, &sb) == 0);

  if (!dir_exists && (mkdir(lmdb_env_path, S_IRWXU) != 0)) {
    printf("Failed to create env dir %s: %s",
           lmdb_env_path, strerror(errno));
    result = -1;
    goto error;
  }

  if ((result = lmdb_storage_init(s, lmdb_env_path) != 0)) {
    printf("Failed to open DB handle");
  } else {
    printf("lmdb storage opened successfully");
    goto cleanup_exit;
  }

  error:
  if (s) {
    lmdb_storage_close(s);
  }

  cleanup_exit:
  free(lmdb_env_path);
  return result;
}

int
lmdb_storage_tx_begin(void* handle)
{
  struct lmdb_storage* s = handle;
  assert(s->txn == NULL);
  return mdb_txn_begin(s->env, NULL, 0, &s->txn);
}

int
lmdb_storage_tx_commit(void* handle)
{
  struct lmdb_storage* s = handle;
  int result;
  assert(s->txn);
  result = mdb_txn_commit(s->txn);
  s->txn = NULL;
  return result;
}

void
lmdb_storage_tx_abort(void* handle)
{
  struct lmdb_storage* s = handle;
  if (s->txn) {
    mdb_txn_abort(s->txn);
    s->txn = NULL;
  }
}

int
lmdb_storage_put(void* handle, paxos_accepted* acc)
{
  struct lmdb_storage* s = handle;
  int result;
  MDB_val key, data;
  char* buffer = paxos_accepted_to_buffer(acc);

  key.mv_data = &acc->iid;
  key.mv_size = sizeof(iid_t);

  data.mv_data = buffer;
  data.mv_size = sizeof(paxos_accepted) + acc->value.paxos_value_len;

  result = mdb_put(s->txn, s->dbi, &key, &data, 0);
  free(buffer);

  return result;
}

int
lmdb_storage_get(void* handle, iid_t iid, paxos_accepted* out)
{
  struct lmdb_storage* s = handle;
  int result;
  MDB_val key, data;

  memset(&data, 0, sizeof(data));

  key.mv_data = &iid;
  key.mv_size = sizeof(iid_t);

  if ((result = mdb_get(s->txn, s->dbi, &key, &data)) != 0) {
    if (result == MDB_NOTFOUND) {
      printf("There is no record for iid: %d\n", iid);
    } else {
      printf("Could not find record for iid: %d : %s\n",
             iid, mdb_strerror(result));
    }
    return 0;
  }

  buffer_to_paxos_accepted(data.mv_data, out);
  assert(iid == out->iid);

  return 1;
}