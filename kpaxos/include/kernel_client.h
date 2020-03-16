#ifndef KERNEL_CLIENT
#define KERNEL_CLIENT
#ifdef user_space
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#endif
#define MSG_LEN 64
#include "paxos_types.h"

typedef struct client_value {
  int bufferId;
  paxos_accepted* value;
} client_value;

struct user_msg {
  size_t size;
  char value[0];
};

struct stats {
  long min_latency;
  long max_latency;
  long avg_latency;
  int delivered_count;
  size_t delivered_bytes;
};

#endif
