//
// Created by roni on 17/08/19.
//

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include "paxos_types.h"
#include <unistd.h>

#include <pthread.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

#define MSG_LEN 64
#define CLIENT_MSG                                                             \
  "********CCCCCCCCLLLLLLLLIIIIIIIIEEEEEEEENNNNNNNNTTTTTTTT*******"

struct timeval tv;
int readMessages = 1000000;
int writeMessages = 100000;

static char*
paxos_accepted_to_buffer(paxos_accepted* acc)
{
    size_t len = acc->value.paxos_value_len;
    char* buffer = malloc(sizeof(paxos_accepted) + len);
    if (buffer == NULL) {
        printf("FUDEU!");
      return NULL;
    }
    memcpy(buffer, acc, sizeof(paxos_accepted));
    if (len > 0) {
        memcpy(&buffer[sizeof(paxos_accepted)], acc->value.paxos_value_val, len);
    }
    return buffer;
}

static void calculateThroughput(){
  struct timeval now;
  double us;

  gettimeofday(&now, NULL);
  us = (now.tv_sec - tv.tv_sec) * 1000000;
  if (us < 0)
    us = 0;

  us += (now.tv_usec - tv.tv_usec);
  fflush(stdout);
  fprintf(stderr, "\n\n%.2lf msgs sent and received per second\n",
          ((float)readMessages * 1000000 / us));
}

void* readProcess(){
    int readDevice = open("/dev/paxos/read-test1", O_WRONLY);
    int fd, ret;
    char *stringToSend;
    if(readDevice < 0) {
        perror("Failed to open the write char device");
        return NULL;
    }

    unsigned int i;
    gettimeofday(&tv, NULL);
    for(i=1;i < readMessages;i++) {
        paxos_accepted *accepted = malloc(sizeof(paxos_accepted));
        memset(accepted, 0, sizeof(paxos_accepted));
        accepted->iid = i;
        stringToSend = paxos_accepted_to_buffer(accepted);

        printf("Writing message to the device bytes=[%zu]: %s.\n", sizeof(paxos_accepted), stringToSend);

        ret = write(readDevice, stringToSend, sizeof(paxos_accepted) + accepted->value.paxos_value_len);

        free(accepted);
        free(stringToSend);
        if (ret < 0) {
            perror("Failed to write message the char device");
            close(readDevice);
            return NULL;
        }
    }
    close(readDevice);
    calculateThroughput();
}

void* writeProcess() {
  int readDevice = open("/dev/paxos/write-test0", O_WRONLY);
  int fd, ret;
  char *stringToSend;
  if(readDevice < 0) {
    perror("Failed to open the write char device");
    return NULL;
  }

  int i;
  for(i=0;i<writeMessages;i++) {
    paxos_accepted *accepted = malloc(sizeof(paxos_accepted));
    accepted->iid = i;
    accepted->value.paxos_value_len = MSG_LEN;
    accepted->value.paxos_value_val = malloc(sizeof(char) * MSG_LEN);
    strcpy(accepted->value.paxos_value_val, CLIENT_MSG);
    stringToSend = paxos_accepted_to_buffer(accepted);

    printf("Writing message to the device bytes=[%zu].\n", sizeof(paxos_accepted));

    ret = write(readDevice, stringToSend, sizeof(paxos_accepted) + accepted->value.paxos_value_len);

    free(accepted);
    free(stringToSend);
    if (ret < 0) {
      perror("Failed to write message the char device");
      close(fd);
      return NULL;
    }
//    sleep(1);
  }

}



int main() {

  pthread_t readThread, writeThread;

  printf("Starting device test code\n");

  pthread_create(&readThread, NULL, readProcess, NULL);
//  pthread_create(&writeThread, NULL, writeProcess, NULL);

  pthread_join(readThread, NULL);
//  pthread_join(writeThread, NULL);

  return 0;
}