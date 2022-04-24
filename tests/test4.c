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
    int n = 1024*1024; // 1 MB
    // int n = 64; // 256 MB
    int pid;

    if(num == 1){
        // sfork_file
        pid = sfork_file((n + 1)* sizeof(int), CHILD_WRITE , &ptr); // 1 GB
    }else if(num == 2){
        // sfork_populate
        pid = sfork((n + 1)* sizeof(int), CHILD_WRITE | SFORK_POPULATE , &ptr); // 1 GB
    }else if(num == 3){
        // sfork
        pid = sfork((n + 1)* sizeof(int), CHILD_WRITE, &ptr); // 1 GB
    }
    

    if(pid < 0)
    {
        printf("fork failed!\n");
        return -1;
    }
    // sleep(4);
    if(pid)
    {
        // parent
        int *arr = (int *) ptr;
        printf("Parent addr = %p\n", ptr);
        
        arr[0] = 100;
    }
    else
    {
        // Child process
        int *arr = (int *) ptr;
        printf("Child addr = %p\n", ptr);
        arr[0] = 100;
    }
    return 0;
}
