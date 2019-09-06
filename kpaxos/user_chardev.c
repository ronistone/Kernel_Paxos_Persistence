#include "kernel_client.h"
#include <fcntl.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSG_LEN 64
#define CLIENT_MSG                                                             \
  "********CCCCCCCCLLLLLLLLIIIIIIIIEEEEEEEENNNNNNNNTTTTTTTT*******"

static int stop = 0, count = 0, verbose = 0;
const char *path;
struct timeval tv;

void usage(const char *name) {
  printf("Usage: %s [options] \n", name);
  printf("Options:\n");
  printf("  %-30s%s\n", "-h, --help", "Output this message and exit");
  printf("  %-30s%s\n", "-c, --chardev_path #", "CharDevice path");
  exit(1);
}

static void run() {
  struct pollfd pol; // 2 events: socket and file
  int len = 0, fd = 0;
  struct user_msg *msg, *recv;

  msg = malloc(sizeof(struct user_msg) + MSG_LEN);
  recv = malloc(sizeof(struct user_msg) + MSG_LEN);
  msg->size = MSG_LEN;
  strcpy(msg->value, CLIENT_MSG);

  // chardevice
  fd = open(path, O_RDWR | O_NONBLOCK, 0);

  if (fd > 0) {
    pol.fd = fd;
    pol.events = POLLIN;
    gettimeofday(&tv, NULL);
    while (!stop) {
      poll(&pol, 1, -1);
      if (pol.revents & POLLIN) {
        ++count;
        len = read(fd, recv, sizeof(struct user_msg) + MSG_LEN);
        if (verbose)
          printf("%10d - Got this: %s\n", count, recv->value);
        if (len) {
          len = write(fd, msg, sizeof(struct user_msg) + MSG_LEN);
          if (verbose)
            printf("%10d - Sent this: %s\n", count, msg->value);
        } else {
          stop = 0;
        }
      } else
        printf("No read event\n");
    }
    close(fd);
  } else
    printf("Error while opening chardev.");

  free(msg);
}

static void check_args(int argc, char *argv[]) {
  int opt = 0, idx = 0;
  static struct option options[] = {{"chardev-path", required_argument, 0, 'c'},
                                    {"help", no_argument, 0, 'h'},
                                    {"verbose", no_argument, 0, 'v'},
                                    {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "c:hv", options, &idx)) != -1) {
    switch (opt) {
    case 'c':
      path = optarg;
      break;
    case 'v':
      verbose = 1;
      break;
    default:
      usage(argv[0]);
    }
  }
}

void stop_execution(int signal) {
  stop = 1;
  struct timeval now;
  double us;

  gettimeofday(&now, NULL);
  us = (now.tv_sec - tv.tv_sec) * 1000000;
  if (us < 0)
    us = 0;

  us += (now.tv_usec - tv.tv_usec);
  fflush(stdout);
  fprintf(stderr, "\n\n%.2lf msgs sent and received per second\n",
          ((float)count * 1000000 / us));
}

int main(int argc, char *argv[]) {
  check_args(argc, argv);
  if (path == NULL)
    usage(argv[0]);

  signal(SIGINT, stop_execution);
  run();
  return 0;
}
