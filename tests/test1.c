#include <stdio.h>
#include <stdlib.h>
#include <sfork.h>
#include <unistd.h>
#include <sys/wait.h>

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
    int n = 1024*1024*256; // 256 MB
    int pid;

    if(num == 1){
        // sfork_file
        pid = sfork_file((n + 1)* sizeof(int), CHILD_WRITE | PARENT_WRITE , &ptr); // 1 GB
    }else if(num == 2){
        // sfork_populate
        pid = sfork((n + 1)* sizeof(int), CHILD_WRITE | PARENT_WRITE | SFORK_POPULATE , &ptr); // 1 GB
    }else if(num == 3){
        // sfork
        pid = sfork((n + 1)* sizeof(int), CHILD_WRITE | PARENT_WRITE, &ptr); // 1 GB
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
        
        arr[n] = 0;
        while(arr[n] == 0);

        printf("Parent: Computing sum\n");
        int sum = 0;
        for(int i = 0; i < n; i++)
            sum += arr[i];
        arr[0] = sum;
        arr[n] = 0;
    }
    else
    {
        // Child process
        int *arr = (int *) ptr;
        printf("Child addr = %p\n", ptr);
        printf("Child: Generating array\n");
        for(int i = 0; i < n; i++)
            arr[i] = i;
        arr[n] = 1;


        while(arr[n] == 1);
        printf("Child: Sum of numbers: %d\n", arr[0]);
    }
    return 0;
}
