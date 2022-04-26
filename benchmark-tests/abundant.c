#include <stdio.h>
#include <stdlib.h>
#include <sfork.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <assert.h>

/*
* This test case allocates large amount of memory
* to sfork and accesses all the allocated pages. Both
* parent and child process access the memory location
* for all pages.
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
    
    if(num > 4){
        printf("Not a valid argument\n");
        return 0;
    }

    void *ptr;
    int n = 1024*1024*mb; // 8 MB
    int np = n/4096;
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
        wait(NULL);
        for(int i = 0;i<np;i++){
            if(arr[i*1024] != i+1)
            {
                printf("arr[i*1024]= %d; i = %d\n", arr[i*1024], i);
                break;
            }
        }
        printf("The value from the child process: %d\n", arr[0]);
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
        // arr[0] = 1000;
        for(int i = 0;i<np;i++){
            arr[i*1024] = i+1;
        }
        if(num == 4)
            shmdt(ptr);
    }

    return 0;
}
