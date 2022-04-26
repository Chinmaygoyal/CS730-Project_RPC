#include<stdio.h>
#include<sfork.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define PAGE_SIZE 4096

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
    int np = 10; // 
    int n = np * PAGE_SIZE; // 10 pages
    int pid, shmid;
    
    if(num == 1){
        // sfork_file
        pid = sfork_file((n + 1)* sizeof(char), CHILD_WRITE | PARENT_WRITE , &ptr); // 1 GB
    }else if(num == 2){
        // sfork_populate
        pid = sfork((n + 1)* sizeof(char), CHILD_WRITE | PARENT_WRITE | SFORK_POPULATE , &ptr); // 1 GB
    }else if(num == 3){
        // sfork
        pid = sfork((n + 1)* sizeof(char), CHILD_WRITE | PARENT_WRITE, &ptr); // 1 GB
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
        printf("Parent PID %d\n", getpid());
        // parent
        wait(NULL);
        char *arr = (char *) ptr;
        for(int i = 0; i < np; i+=2)
            arr[i * PAGE_SIZE] = i;
        for(int i = 1; i < np; i+=2)
        {
            if(arr[i * PAGE_SIZE] != i)
            {
                printf("arr[i * PAGE_SIZE] != i, i = %d\n", i);
                break;
            }
        }
        // sleep(5);
    }
    else
    {
        // Child process
        printf("Child PID %d\n", getpid());
        char *arr = (char *) ptr;
        for(int i = 1; i < np; i+=2)
            arr[i * PAGE_SIZE] = i;
    }
    return 0;
}
