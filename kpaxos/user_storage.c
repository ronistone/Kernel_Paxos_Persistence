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


static int stop = 0, verbose = 0;
const char *read_device_path, *write_device_path;
pthread_t read_thread, write_thread;


void usage(const char *name) {
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

//static void* read_storage_thread(void* param) {
//
//  int fd = 1;
//  struct pollfd polling;
//  struct user_msg *msg, *recv;
//  int len = 0, count = 0;
//
//  msg = malloc(sizeof(struct user_msg) + MSG_LEN);
//  recv = malloc(sizeof(struct user_msg) + MSG_LEN);
//  msg->size = MSG_LEN;
//  strcpy(msg->value, CLIENT_MSG);
//
////  fd = open(read_device_path, O_RDWR | O_NONBLOCK, 0);
//
//  if (fd > 0) {
////    polling.fd = fd;
////    polling.events = POLLIN;
//    while (!stop) {
//        printf("Doing Read...\n");
//        sleep(2);
////      poll(&polling, 1, -1);
//      if (polling.revents & POLLIN) {
////        len = read(fd, recv, sizeof(struct user_msg) + MSG_LEN);
//        if (len) {
//          if (verbose)
//            printf("%10d - Got this: %s\n", count, recv->value);
////          // do find on lmdb
////          len = write(fd, msg, sizeof(struct user_msg) + MSG_LEN);
////          if (verbose)
////            printf("%10d - Sent this: %s\n", count, msg->value);
//        }
//      } else {
//        printf("No read event\n");
//      }
//    }
////    close(fd);
//  } else {
//    printf("Error while opening the read storage chardev.\n");
//  }
//  free(msg);
//  return NULL;
//}

static paxos_accepted storage_get(struct lmdb_storage lmdbStorage, uint32_t id){

  if(lmdb_storage_tx_begin((void*) &lmdbStorage) != 0){
    printf("Fail to open transaction!\n");
  }
  paxos_accepted out;
  if(lmdb_storage_get((void*) &lmdbStorage, id, &out)!=1){
    printf("Fail to get in storage!\n");
    lmdb_storage_tx_abort((void*) &lmdbStorage);
  }

  if(lmdb_storage_tx_commit((void*) &lmdbStorage) != 0){
    printf("Fail to commit transaction!\n");
  }
  return out;
}

static int storage_put(struct lmdb_storage lmdbStorage, paxos_accepted* accepted){
  if(lmdb_storage_tx_begin((void*) &lmdbStorage) != 0){
    printf("Fail to open transaction!\n");
    return 1;
  }

  if(lmdb_storage_put((void*) &lmdbStorage, accepted)!=0){
    printf("Fail to put in storage!\n");
    lmdb_storage_tx_abort((void*) &lmdbStorage);
  }

  if(lmdb_storage_tx_commit((void*) &lmdbStorage) != 0){
    printf("Fail to commit transaction!\n");
    return 1;
  }

  return 0;
}

static void* write_storage_thread(void* param) {
  struct lmdb_storage lmdbStorage;
  lmdb_storage_open((void*) &lmdbStorage);
  int fd = 1;
  struct pollfd polling;
  char *recv;
  paxos_accepted* accepted;
  int len = 0;

  recv = malloc(sizeof(paxos_accepted));
  accepted = malloc(sizeof(paxos_accepted));

  printf("%s\n", write_device_path);
  fd = open(write_device_path, O_RDWR | O_NONBLOCK, 0);

  uint32_t id = 0;
  if (fd > 0) {
    polling.fd = fd;
    polling.events = POLLIN;
    while (!stop) {
      printf("Check write device...\n");
      poll(&polling, 1, 2000);
      if (polling.revents & POLLIN) {
        len = read(fd, recv, sizeof(paxos_accepted));
        buffer_to_paxos_accepted(recv, accepted);
        if (verbose)
          printf("Got this: %d\n", accepted->iid);
        if (len) {
          storage_put(lmdbStorage, accepted);
        }
      } else {
        printf("No read event\n");

        paxos_accepted out = storage_get(lmdbStorage, id++);
        printf("found this paxos_accepted %d\n", out.iid);

      }
    }
    lmdb_storage_close((void*) &lmdbStorage);
    close(fd);
  } else {
    printf("Error while opening the write storage chardev.\n");
  }
  free(recv);

  pthread_exit(0);
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


  run();
  return 0;
}
