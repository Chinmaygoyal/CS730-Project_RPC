#include <stdio.h>
#include <stdlib.h>
#include <sfork.h>
#include <unistd.h>
#include <sys/wait.h>

/*
* This test case allocates large amount of memory
* to sfork but access random pages for child and parent.
* Currently accessing 1/4th pages of the allocated number
* of pages. The pages may be different for parent and child
* process.
*/

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
    int n = 1024*1024*8; // 8 MB
    int pid;

    if(num == 1){
        // sfork_file
        pid = sfork_file(n, CHILD_WRITE | PARENT_WRITE , &ptr); // 8 MB
    }else if(num == 2){
        // sfork_populate
        pid = sfork(n, CHILD_WRITE | PARENT_WRITE | SFORK_POPULATE , &ptr); // 8 MB
    }else if(num == 3){
        // sfork
        pid = sfork(n, CHILD_WRITE | PARENT_WRITE, &ptr); // 8 MB
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
        for(int i = 0;i<512;i++){
            int page_number = rand()%2048;
            arr[page_number*1024] = page_number;
        }
        wait(NULL);
        printf("The value from the child process: %d\n",arr[0]);
    }
    else
    {
        // Child process
        int *arr = (int *) ptr;
        printf("Child addr = %p\n", ptr);
        for(int i = 0;i<512;i++){
            int page_number = rand()%2048;
            arr[page_number*1024] = page_number;
        }
    }

    return 0;
}
