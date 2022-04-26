#include <stdio.h>
#include <stdlib.h>
#include <sfork.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h> 
#include <assert.h>

int main(int argc, char* argv[])
{

    if(argc < 2){
        printf("Please tell which implementation of sfork to use\n");
        return 0;
    }

    char *a = argv[1];
    int num = atoi(a);
    if(num > 4){
        printf("Not a valid argument\n");
        return 0;
    }

    void *ptr;
    int n = 100; // 8 MB
    int pid, shmid;

    if(num == 1){
        // sfork_file
        pid = sfork_file(n * sizeof(int), CHILD_WRITE | PARENT_WRITE , &ptr); // 1 GB
    }else if(num == 2){
        // sfork_populate
        pid = sfork(n * sizeof(int), CHILD_WRITE | PARENT_WRITE | SFORK_POPULATE , &ptr); // 1 GB
    }else if(num == 3){
        // sfork
        pid = sfork(n * sizeof(int), CHILD_WRITE | PARENT_WRITE, &ptr); // 1 GB
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

    if(pid)
    {
        // parent
        int *arr = (int *) ptr;
        printf("Parent addr = %p\n", ptr);
        arr[0] = 11;
        printf("Parent: Waiting for child\n");
        wait(NULL);
        printf("Parent: arr[0]: %d\n", arr[0]);
        long sum = 0;
        for(int i = 0; i < n; i++)
            sum += arr[i];
        printf("Parent: Sum of numbers: %ld\n", sum);
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
        printf("Child: arr[0]: %d\n", arr[0]);
        for(int i = 0; i < n; i++)
            arr[i] = i+1;
        arr[0] = 1;
        if(num == 4)
            shmdt(ptr);
    }
    return 0;
}
