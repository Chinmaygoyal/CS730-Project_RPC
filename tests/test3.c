#include<stdio.h>
#include<sfork.h>
#include <unistd.h>
#include<stdlib.h>
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
    if(num > 3){
        printf("Not a valid argument\n");
        return 0;
    }


    void *ptr;
    int n = 5;
    /*
    * Create 10 child/grandchild processes with only the first being through sfork.
    */
    int pid;

    if(num == 1){
        // sfork_file
        pid = sfork_file((n+1)*4096, CHILD_WRITE | PARENT_WRITE , &ptr); // 1 GB
    }else if(num == 2){
        // sfork_populate
        pid = sfork((n+1)*4096, CHILD_WRITE | PARENT_WRITE | SFORK_POPULATE , &ptr); // 1 GB
    }else if(num == 3){
        // sfork
        pid = sfork((n+1)*4096, CHILD_WRITE | PARENT_WRITE, &ptr); // 1 GB
    }

    if(pid == 0){
        // child process
        for(int i = 0;i<4;i++){
            int pid_1 = fork();
        }
        int *arr = (int *) ptr;
        for(int i = 0; i < n; i++)
            arr[i] = i;
        arr[n] = 1;
        int sum = 0;
        for(int i = 0;i<n;i++){
            sum += arr[i];
        }
        printf("The current process id is: %d. The sum is: %d\n",getpid(),sum);
    }else{
        int *arr = (int *) ptr;
        printf("Parent addr = %p\n", ptr);
        
        arr[n] = 0;
        while(arr[n] == 0);

        printf("Parent: Computing sum\n");
        int sum = 0;
        for(int i = 0; i < n; i++)
            sum += arr[i];
        arr[0] = sum;
        arr[n] = 0;
        printf("The current process id is: %d. The sum is: %d\n",getpid(),sum);
    }
}
