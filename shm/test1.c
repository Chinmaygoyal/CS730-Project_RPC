#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

/* Test Case to show that the 
shared memory regions are inherited
from parent to child process */

int main(){

   int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
   void *mem = shmat(shmid, NULL, 0);
   *(int *)mem = 100;
   
   int pid = fork();
   
   if (pid == 0) { // child process
      // *(int *)mem = 89; 
      printf("%d\n", *(int *)mem);
      shmdt(mem);
      return 0; 
   } else { // parent process
      // wait(NULL);
      printf("%d\n", *(int *)mem);
      while (1){}
      shmdt(mem);
      shmctl(shmid, IPC_RMID, NULL);
      return 0; 
   }

   return 0;
}