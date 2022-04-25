#include <stdio.h>
#include <stdlib.h>
#include <sfork.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

/*
* This test case allocates large amount of memory
* to sfork but only accesses a small chunk. Both
* parent and child process access the memory location
* for two pages.
*/

int main(int argc, char* argv[])
{

    if(argc < 2){
        printf("Please tell which implementation of sfork to use and size of shared mem in MB\n");
        return 0;
    }

    char *a = argv[1];
    int num = atoi(a);
    int mb = atoi(argv[2]);
    
    if(num > 3){
        printf("Not a valid argument\n");
        return 0;
    }

    void *ptr;
    int n = 1024*1024*mb; // 8 MB
    int pid, shmid;

    if(num == 1){
        // sfork_file
        pid = sfork_file(n, CHILD_WRITE | PARENT_WRITE , &ptr); // 8 MB
    }else if(num == 2){
        // sfork_populate
        pid = sfork(n, CHILD_WRITE | PARENT_WRITE | SFORK_POPULATE , &ptr); // 8 MB
    }else if(num == 3){
        // sfork
        pid = sfork(n, CHILD_WRITE | PARENT_WRITE, &ptr); // 8 MB
    }else if(num == 4){
        shmid = shmget(IPC_PRIVATE, n, IPC_CREAT | 0600);
        ptr = shmat(shmid, NULL, 0);
        pid = fork();
    }

    if(pid < 0)
    {
        printf("fork failed!\n");
        return -1;
    }
    // sleep(4);
    
    if(pid){
        // parent
        int *arr = (int *) ptr;
        printf("Parent addr = %p\n", ptr);
        arr[4096] = 100;
        arr[3*4096] = 100;
        arr[5*4096] = 100;
        wait(NULL);
        printf("The value from the child process: %d\n",arr[0]);
        if(num == 4) {
            shmdt(ptr);
            shmctl(shmid, IPC_RMID, NULL);
        }
    }
    else
    {
        // Child process
        int *arr = (int *) ptr;
        printf("Child addr = %p\n", ptr);
        arr[0] = 100;
        arr[2*4096] = 100;
        arr[4*4096] = 100;
        if(num == 4)
            shmdt(ptr);
    }

    return 0;
}
