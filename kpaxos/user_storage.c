//
// Created by roni on 30/09/19.
//

#include <fcntl.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "kernel_client.h"
#include "paxos.h"
#include "lmdb_operations.h"

#define WHATEVER_VALUE 0
#define LOG(isRead, fmt, args...)                                                \
  isRead? printf("READ: " fmt "\n", ##args): printf("WRITE: " fmt "\n", ##args)

static int stop = 0, verbose = 0;
static int READ = 1;
static int WRITE = 0;
static const char *read_device_path, *write_device_path;
static pthread_t read_thread, write_thread;


static void usage(const char *name) {
  printf("Usage: %s [options] \n", name);
  printf("Options:\n");
  printf("  %-30s%s\n", "-h, --help", "Output this message and exit");
  printf("  %-30s%s\n", "-r, --read-chardev_path #", "Kernel paxos lmdb query kernel device path");
  printf("  %-30s%s\n", "-w, --write-chardev_path #", "Kernel paxos lmdb write kernel device path");
  exit(1);
}

static void check_args(int argc, char *argv[]) {
  int opt = 0, idx = 0;
  static struct option options[] = {{"write-chardev-path", required_argument, 0, 'w'},
                                    {"read-chardev-path", required_argument, 0, 'r'},
                                    {"help", no_argument, 0, 'h'},
                                    {"verbose", no_argument, 0, 'v'},
                                    {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "w:r:hv", options, &idx)) != -1) {
    switch (opt) {
    case 'w':
      write_device_path = optarg;
      break;
    case 'r':
      read_device_path = optarg;
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

static paxos_accepted* storage_get(struct lmdb_storage lmdbStorage, uint32_t id){

  if(lmdb_storage_tx_begin( &lmdbStorage) != 0){
    printf("Fail to open transaction!\n");
  }
  paxos_accepted* out = NULL;
  if(lmdb_storage_get( &lmdbStorage, id, out)!=1){
    printf("Fail to get in storage!\n");
    lmdb_storage_tx_abort( &lmdbStorage);
    return NULL;
  }

  if(lmdb_storage_tx_commit( &lmdbStorage) != 0){
    printf("Fail to commit transaction!\n");
  }
  return out;
}

static int storage_put(struct lmdb_storage lmdbStorage, paxos_accepted* accepted){
  if(lmdb_storage_tx_begin( &lmdbStorage) != 0){
    printf("Fail to open transaction!\n");
    return 1;
  }

  if(lmdb_storage_put( &lmdbStorage, accepted)!=0){
    printf("Fail to put in storage!\n");
    lmdb_storage_tx_abort( &lmdbStorage);
  }

  if(lmdb_storage_tx_commit( &lmdbStorage) != 0){
    printf("Fail to commit transaction!\n");
    return 1;
  }

  return 0;
}

//static void* read_storage_thread(void* param) {
//  struct lmdb_storage lmdbStorage;
//  if(lmdb_storage_open( &lmdbStorage) != 0){
//    printf("Fail to open storage on read thread\n");
//    pthread_exit(0);
//    return NULL;
//  }
//  int fd = 1;
//  struct pollfd polling;
//  char *recv;
//  paxos_accepted* accepted;
//  int len = 0;
//
//  recv = malloc(sizeof(paxos_accepted) + (sizeof(char) * MAX_PAXOS_VALUE_SIZE));
//  accepted = malloc(sizeof(paxos_accepted) + (sizeof(char) * MAX_PAXOS_VALUE_SIZE));
//
//  printf("READ: %s\n", read_device_path);
//  fd = open(read_device_path, O_RDWR | O_NONBLOCK, 0);
//
//  if (fd > 0) {
//    polling.fd = fd;
//    polling.events = POLLIN;
//    while (!stop) {
//      printf("READ: Check read device...\n");
//      poll(&polling, 1, 2000);
//      if (polling.revents & POLLIN) {
//        len = read(fd, recv, WHATEVER_VALUE);
//        buffer_to_paxos_accepted(recv, accepted);
//        if (verbose)
//          printf("READ: Got this: %d  --> %s\n", accepted->iid, accepted->value.paxos_value_val);
//        if (len) {
//          paxos_accepted* out = storage_get(lmdbStorage, accepted->iid);
//
//          if(out != NULL) {
//            printf("READ: found %d, sending to LKM\n", out -> iid);
//            char* msg = paxos_accepted_to_buffer(out);
//            write(fd, msg, sizeof(paxos_accepted) + out->value.paxos_value_len);
//          } else {
//            printf("READ: not found %d, sending not found to LKM\n", accepted -> iid);
//            out = malloc(sizeof(paxos_accepted));
//            out->iid = -1;
//            char* msg = paxos_accepted_to_buffer(out);
//            write(fd, msg, sizeof(paxos_accepted));
//          }
//
//        }
//      } else {
//        printf("READ: No read event\n");
//      }
//    }
//    lmdb_storage_close( &lmdbStorage);
//    close(fd);
//  } else {
//    printf("READ: Error while opening the write storage chardev.\n");
//  }
//  free(recv);
//
//  pthread_exit(0);
//}
//
//static void* write_storage_thread(void* param) {
//  struct lmdb_storage lmdbStorage;
//  if(lmdb_storage_open( &lmdbStorage) != 0){
//    printf("Fail to open storage on write thread\n");
//    pthread_exit(0);
//    return NULL;
//  }
//  int fd = 1;
//  struct pollfd polling;
//  char *recv;
//  paxos_accepted* accepted;
//  int len = 0;
//
//  recv = malloc(sizeof(paxos_accepted) + (sizeof(char) * MAX_PAXOS_VALUE_SIZE));
//  accepted = malloc(sizeof(paxos_accepted) + (sizeof(char) * MAX_PAXOS_VALUE_SIZE));
//
//  printf("WRITE: %s\n", write_device_path);
//  fd = open(write_device_path, O_RDWR | O_NONBLOCK, 0);
//
//  if (fd > 0) {
//    polling.fd = fd;
//    polling.events = POLLIN;
//    while (!stop) {
//      printf("WRITE: Check write device...\n");
//      poll(&polling, 1, 2000);
//      if (polling.revents & POLLIN) {
//        len = read(fd, recv, WHATEVER_VALUE);
//        buffer_to_paxos_accepted(recv, accepted);
//        if (verbose){
//            printf("WRITE: Got this: %d  --> %s   -> %p\n", accepted->iid, accepted->value.paxos_value_val, accepted->value.paxos_value_val);
//        }
//        if (len) {
//          storage_put(lmdbStorage, accepted);
//        }
//      } else {
//        printf("WRITE: No read event\n");
//      }
//    }
//    lmdb_storage_close( &lmdbStorage);
//    close(fd);
//  } else {
//    printf("WRITE: Error while opening the write storage chardev.\n");
//  }
//  free(recv);
//
//  pthread_exit(0);
//}

static void process_write_message(struct lmdb_storage lmdbStorage, paxos_accepted* accepted){
  storage_put(lmdbStorage, accepted);
}

static void process_read_message(struct lmdb_storage lmdbStorage, paxos_accepted* accepted, int fd){
  paxos_accepted* out = storage_get(lmdbStorage, accepted->iid);

  if(out != NULL) {
    LOG(READ, "found %d, sending to LKM\n", out -> iid);
    char* msg = paxos_accepted_to_buffer(out);
    write(fd, msg, sizeof(paxos_accepted) + out->value.paxos_value_len);
  } else {
    LOG(READ, "not found %d, sending not found to LKM\n", accepted -> iid);
    out = malloc(sizeof(paxos_accepted));
    out->iid = -1;
    char* msg = paxos_accepted_to_buffer(out);
    write(fd, msg, sizeof(paxos_accepted));
  }

}

static void* generic_storage_thread(void* param) {
  int isRead = *((int*)param);
  const char* device_path;
  if(isRead){
    device_path = read_device_path;
  } else {
    device_path = write_device_path;
  }


  struct lmdb_storage lmdbStorage;
  if(lmdb_storage_open( &lmdbStorage) != 0){
    LOG(isRead, "Fail to open storage on write thread");
    pthread_exit(0);
    return NULL;
  }
  int fd = 1;
  struct pollfd polling;
  char *recv;
  paxos_accepted* accepted;
  int len = 0;

  recv = malloc(sizeof(paxos_accepted) + (sizeof(char) * MAX_PAXOS_VALUE_SIZE));
  accepted = malloc(sizeof(paxos_accepted) + (sizeof(char) * MAX_PAXOS_VALUE_SIZE));

  LOG(isRead, "%s", device_path);
  fd = open(device_path, O_RDWR | O_NONBLOCK, 0);

  if (fd > 0) {
    polling.fd = fd;
    polling.events = POLLIN;
    while (!stop) {
      LOG(isRead, "Check write device...");
      poll(&polling, 1, 2000);
      if (polling.revents & POLLIN) {
        len = read(fd, recv, WHATEVER_VALUE);
        buffer_to_paxos_accepted(recv, accepted);
        if (verbose){
          LOG(isRead, "Got this: %d  --> %s   -> %p", accepted->iid, accepted->value.paxos_value_val, accepted->value.paxos_value_val);
        }
        if (len) {
          if(isRead){
            process_read_message(lmdbStorage, accepted, fd);
          } else {
            process_write_message(lmdbStorage, accepted);
          }
        }
      } else {
        LOG(isRead, "No read event");
      }
    }
    lmdb_storage_close( &lmdbStorage);
    close(fd);
  } else {
    LOG(isRead, "Error while opening the write storage chardev.");
  }
  free(recv);

  pthread_exit(0);
}

static void run(){

//    pthread_create(&read_thread, NULL, read_storage_thread, NULL);
//    pthread_create(&write_thread, NULL, write_storage_thread, NULL);

    pthread_create(&read_thread, NULL, generic_storage_thread, &READ);
    pthread_create(&write_thread, NULL, generic_storage_thread, &WRITE);

    pthread_join(read_thread, NULL);
    pthread_join(write_thread,NULL);
}

int main(int argc, char *argv[]) {
  check_args(argc, argv);
  if (read_device_path == NULL || write_device_path == NULL)
    usage(argv[0]);
  signal(SIGINT, stop_execution);


  run();
  return 0;
}
