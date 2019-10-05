//
// Created by roni on 30/09/19.
//

#include "kernel_client.h"
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

#define MSG_LEN 64
#define CLIENT_MSG                                                             \
  "********CCCCCCCCLLLLLLLLIIIIIIIIEEEEEEEENNNNNNNNTTTTTTTT*******"

static int stop = 0, count = 0, verbose = 0;
const char *read_device_path, *write_device_path;
pthread_t read_thread, write_thread;

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


static void read_storage_thread(void) {

  int fd = 1;
  struct pollfd polling;
  struct user_msg *msg, *recv;
  int len = 0;

  msg = malloc(sizeof(struct user_msg) + MSG_LEN);
  recv = malloc(sizeof(struct user_msg) + MSG_LEN);
  msg->size = MSG_LEN;
  strcpy(msg->value, CLIENT_MSG);

//  fd = open(read_device_path, O_RDWR | O_NONBLOCK, 0);

  if (fd > 0) {
    polling.fd = fd;
    polling.events = POLLIN;
    while (!stop) {
        printf("Doing Read...\n");
        sleep(2);
//      poll(&polling, 1, -1);
//      if (polling.revents & POLLIN) {
//        len = read(fd, recv, sizeof(struct user_msg) + MSG_LEN);
//        if (verbose)
//          printf("%10d - Got this: %s\n", count, recv->value);
//        if (len) {
//          // do find on lmdb
//          len = write(fd, msg, sizeof(struct user_msg) + MSG_LEN);
//          if (verbose)
//            printf("%10d - Sent this: %s\n", count, msg->value);
//        }
//      } else {
//        printf("No read event\n");
//      }
    }
//    close(fd);
  } else {
    printf("Error while opening the read storage chardev.\n");
  }
  free(msg);
}

static void write_storage_thread() {
  int fd = 1;
  struct pollfd polling;
  struct user_msg *msg, *recv;
  int len = 0;

  msg = malloc(sizeof(struct user_msg) + MSG_LEN);
  recv = malloc(sizeof(struct user_msg) + MSG_LEN);
  msg->size = MSG_LEN;
  strcpy(msg->value, CLIENT_MSG);

//  fd = open(write_device_path, O_RDWR | O_NONBLOCK, 0);

  if (fd > 0) {
    polling.fd = fd;
    polling.events = POLLIN;
    while (!stop) {
        printf("Doing Write...\n");
        sleep(2);
//      poll(&polling, 1, -1);
//      if (polling.revents & POLLIN) {
//        len = read(fd, recv, sizeof(struct user_msg) + MSG_LEN);
//        if (verbose)
//          printf("%10d - Got this: %s\n", count, recv->value);
//        if (len) {
//          // do save on lmdb
//        }
//      } else {
//        printf("No read event\n");
//      }
    }
//    close(fd);
  } else {
    printf("Error while opening the write storage chardev.\n");
  }
  free(msg);
}

static void run(){

    pthread_create(&read_thread, NULL, read_storage_thread, NULL);
    pthread_create(&write_thread, NULL, write_storage_thread, NULL);

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
