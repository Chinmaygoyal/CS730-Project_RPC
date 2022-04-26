#include<stdio.h>
#include<sfork.h>
#include <unistd.h>
#include<stdlib.h>
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
    int n = 5;
    /*
    * Create 8 child/grandchild processes with only the first being through sfork.
    */
    int pid, shmid;

    if(num == 1){
        // sfork_file
        pid = sfork_file((n+1)*4096, PARENT_WRITE , &ptr); // 1 GB
    }else if(num == 2){
        // sfork_populate
        pid = sfork((n+1)*4096, PARENT_WRITE | SFORK_POPULATE , &ptr); // 1 GB
    }else if(num == 3){
        // sfork
        pid = sfork((n+1)*4096, PARENT_WRITE, &ptr); // 1 GB
    }else if(num == 4){
        shmid = shmget(IPC_PRIVATE, n, IPC_CREAT | 0600);
        ptr = shmat(shmid, NULL, 0);
        pid = fork();
    }
    
    
    if(pid == 0){
        // child process
        int *arr = (int *) ptr;
        sleep(1);
        while(arr[n] == 0);

        for(int i = 0;i<3;i++){
            int pid_1 = fork();
        }

        int sum = 0;
        for(int i = 0;i<n;i++){
            sum += arr[i];
        }
        printf("The current process id is: %d. The sum is: %d\n",getpid(), sum);
        if(num == 4)
            shmdt(ptr);
    }else{
        int *arr = (int *) ptr;
        arr[n] = 0;
        printf("Parent addr = %p\n", ptr);
        
        for(int i = 0; i < n; i++)
            arr[i] = i;
        
        arr[n] = 1;
        
        printf("Parent: Computing sum\n");
        int sum = 0;
        for(int i = 0; i < n; i++)
            sum += arr[i];
        printf("The parent process id is: %d. The sum is: %d\n",getpid(),sum);
        if(num == 4) {
            shmdt(ptr);
            shmctl(shmid, IPC_RMID, NULL);
        }
    }
}
