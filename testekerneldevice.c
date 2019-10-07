//
// Created by roni on 17/08/19.
//

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>

#include <pthread.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

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

void* writeProcess() {
  int readDevice = open("/dev/paxos/persistence0", O_WRONLY);
  int fd, ret;
  char stringToSend[BUFFER_LENGTH];
  if(readDevice < 0) {
    perror("Failed to open the write char device");
    return NULL;
  }

  while(1) {
    printf("Type the message to send kernel module\n");
    scanf("%[^\n]%*c", stringToSend);
    printf("Writing message to the device [%s].\n", stringToSend);

    ret = write(readDevice, stringToSend, strlen(stringToSend));

    if (ret < 0) {
      perror("Failed to write message the char device");
      close(fd);
      return NULL;
    }
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