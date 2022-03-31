#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int create_handle(){
    int fd = open("/dev/sfork_dev",O_RDWR);
    return fd;
}

void close_handle(int cdev){
    close(cdev);
    return;
}

/*
Main function to perform sfork.
    size: The size of the shared memory between child and parent
    direction: Unidirectional or Bidirectional
*/
int sfork(uint64_t size, int direction){
    /*
    Call fork from parent and then initialize the memory of the child and the parent
    with the same pfn pages.
    */
    int pid = fork();
    return pid;
}

