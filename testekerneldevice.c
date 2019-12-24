//
// Created by roni on 17/08/19.
//

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include "paxos_types.h"

#include <pthread.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

#define MSG_LEN 64
#define CLIENT_MSG                                                             \
  "********CCCCCCCCLLLLLLLLIIIIIIIIEEEEEEEENNNNNNNNTTTTTTTT*******"


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

void* readProcess(){
    int readDevice = open("/dev/paxos/read-test1", O_WRONLY);
    int fd, ret;
    char *stringToSend;
    if(readDevice < 0) {
        perror("Failed to open the write char device");
        return NULL;
    }

    unsigned int i;
    for(i=1;i<100000;i++) {
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
            close(fd);
            return NULL;
        }
    }

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
  for(i=0;i<10000;i++) {
    paxos_accepted *accepted = malloc(sizeof(paxos_accepted));
    accepted->iid = i;
    accepted->value.paxos_value_len = MSG_LEN;
    accepted->value.paxos_value_val = malloc(sizeof(char) * MSG_LEN);
    strcpy(accepted->value.paxos_value_val, CLIENT_MSG);
    stringToSend = paxos_accepted_to_buffer(accepted);

    printf("Writing message to the device bytes=[%zu].\n", sizeof(paxos_accepted));

    ret = write(readDevice, stringToSend, sizeof(paxos_accepted) + accepted->value.paxos_value_len);

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