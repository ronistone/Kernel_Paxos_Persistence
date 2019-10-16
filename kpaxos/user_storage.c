//
// Created by roni on 30/09/19.
//

#include "kernel_client.h"
#include "paxos.h"
#include <fcntl.h>
#include <lmdb.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <sys/stat.h>


#define DEFAULT_SIZE 10*1024*1024
#define MSG_LEN 64
#define CLIENT_MSG                                                             \
  "********CCCCCCCCLLLLLLLLIIIIIIIIEEEEEEEENNNNNNNNTTTTTTTT*******"

static int stop = 0, verbose = 0;
const char *read_device_path, *write_device_path, *db_env_path = "/tmp/paxos";
pthread_t read_thread, write_thread;
size_t lmdb_mapsize = DEFAULT_SIZE;

struct lmdb_storage
{
    MDB_env* env;
    MDB_txn* txn;
    MDB_dbi dbi;
};

void usage(const char *name) {
  printf("Usage: %s [options] \n", name);
  printf("Options:\n");
  printf("  %-30s%s\n", "-h, --help", "Output this message and exit");
  printf("  %-30s%s\n", "-r, --read-chardev_path #", "Kernel paxos lmdb query kernel device path");
  printf("  %-30s%s\n", "-w, --write-chardev_path #", "Kernel paxos lmdb write kernel device path");
  printf("  %-30s%s%d\n", "-m, --lmdb-mapsize #", "lmdb database limit size by default it is ", DEFAULT_SIZE);
  exit(1);
}

static void check_args(int argc, char *argv[]) {
  int opt = 0, idx = 0;
  static struct option options[] = {{"write-chardev-path", required_argument, 0, 'w'},
                                    {"read-chardev-path", required_argument, 0, 'r'},
                                    {"lmdb-mapsize", required_argument, 0, 'm'},
                                    {"help", no_argument, 0, 'h'},
                                    {"verbose", no_argument, 0, 'v'},
                                    {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "w:r:m:f:hv", options, &idx)) != -1) {
    switch (opt) {
    case 'w':
      write_device_path = optarg;
      break;
    case 'r':
      read_device_path = optarg;
      break;
    case 'm':
      lmdb_mapsize = atoi(optarg);
      break;
    case 'v':
      verbose = 1;
      break;
    default:
      usage(argv[0]);
    }
  }
}

static void stop_execution(int signal) {
  stop = 1;
  printf("Finishing Threads...\n");
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

static int
lmdb_storage_init(struct lmdb_storage* s)
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

static void* read_storage_thread(void* param) {

  int fd = 1;
  struct pollfd polling;
  struct user_msg *msg, *recv;
  int len = 0, count = 0;

  msg = malloc(sizeof(struct user_msg) + MSG_LEN);
  recv = malloc(sizeof(struct user_msg) + MSG_LEN);
  msg->size = MSG_LEN;
  strcpy(msg->value, CLIENT_MSG);

//  fd = open(read_device_path, O_RDWR | O_NONBLOCK, 0);

  if (fd > 0) {
//    polling.fd = fd;
//    polling.events = POLLIN;
    while (!stop) {
        printf("Doing Read...\n");
        sleep(2);
//      poll(&polling, 1, -1);
      if (polling.revents & POLLIN) {
//        len = read(fd, recv, sizeof(struct user_msg) + MSG_LEN);
        if (len) {
          if (verbose)
            printf("%10d - Got this: %s\n", count, recv->value);
//          // do find on lmdb
//          len = write(fd, msg, sizeof(struct user_msg) + MSG_LEN);
//          if (verbose)
//            printf("%10d - Sent this: %s\n", count, msg->value);
        }
      } else {
        printf("No read event\n");
      }
    }
//    close(fd);
  } else {
    printf("Error while opening the read storage chardev.\n");
  }
  free(msg);
  return NULL;
}

static void* write_storage_thread(void* param) {
  int fd = 1;
  struct pollfd polling;
  struct user_msg *msg, *recv;
  int len = 0, count = 0;

  msg = malloc(sizeof(struct user_msg) + MSG_LEN);
  recv = malloc(sizeof(struct user_msg) + MSG_LEN);
  msg->size = MSG_LEN;
  strcpy(msg->value, CLIENT_MSG);

  printf("%s\n", write_device_path);
  fd = open(write_device_path, O_RDWR | O_NONBLOCK, 0);

  if (fd > 0) {
    polling.fd = fd;
    polling.events = POLLIN;
    while (!stop) {
        printf("Check write device...\n");
//        sleep(2);
      poll(&polling, 1, -1);
      if (polling.revents & POLLIN) {
        len = read(fd, recv, sizeof(struct user_msg) + MSG_LEN);
        if (verbose)
          printf("%10d - Got this: %s\n", count, recv->value);
        if (len) {
          // do save on lmdb
        }
      } else {
        printf("No read event\n");
      }
    }
    close(fd);
  } else {
    printf("Error while opening the write storage chardev.\n");
  }
  free(msg);
  return NULL;
}

static void run(){

//    pthread_create(&read_thread, NULL, read_storage_thread, NULL);
    pthread_create(&write_thread, NULL, write_storage_thread, NULL);

//    pthread_join(read_thread, NULL);
    pthread_join(write_thread,NULL);
}

int main(int argc, char *argv[]) {
  check_args(argc, argv);
  if (read_device_path == NULL || write_device_path == NULL)
    usage(argv[0]);
  signal(SIGINT, stop_execution);

  struct lmdb_storage lmdbStorage;
  lmdb_storage_init(&lmdbStorage);
//  run();
  return 0;
}
