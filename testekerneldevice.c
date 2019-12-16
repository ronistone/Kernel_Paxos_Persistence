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


void* readProcess(){
  int fd, ret;
  int readDevice = open("/dev/paxos/test_persistence_write", O_RDWR);

  if(fd < 0) {
    perror("Failed to open the read char device");
    return NULL;
  }

  printf("Reading from device\n");
  while(1) {
    ret = read(fd, receive, BUFFER_LENGTH);

    if(ret < 0 ) {
      perror("Failed to read message from the char device");
      printf("End of the program\n");
      return NULL;
    }
    receive[ret] = '\0';
    printf("The received message is: [%s]\n", receive);
  }

}

static char*
paxos_accepted_to_buffer(paxos_accepted* acc)
{
  size_t len = acc->value.paxos_value_len;
  char* buffer = malloc(sizeof(paxos_accepted) + len);
//  char* buffer = malloc(sizeof(paxos_accepted));
  memset(buffer, 0, sizeof(paxos_accepted));
  if (buffer == NULL) {
    printf("Fudeu!\n");
    return NULL;
  }
  memcpy(buffer, acc, sizeof(paxos_accepted));
  if (len > 0) {
    memcpy(&buffer[sizeof(paxos_accepted)], acc->value.paxos_value_val, len);
  }

  printf("%d\n", acc->iid);
  int i;
  for(i=0;i< sizeof(paxos_accepted);i++)
    printf("%d", buffer[i]);
  return buffer;
}

void* writeProcess() {
  int readDevice = open("/dev/paxos/persistence0", O_WRONLY);
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

//  pthread_create(&readThread, NULL, readProcess, NULL);
  pthread_create(&writeThread, NULL, writeProcess, NULL);

//  pthread_join(readThread, NULL);
  pthread_join(writeThread, NULL);

  return 0;
}