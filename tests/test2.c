#include<stdio.h>
#include<sfork.h>
#include<stdlib.h>
#include <unistd.h>
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
    int np = 600; // 
    /* 
     * SOLVED: By using mmgrab() and mmdrop()
     * On large values of np (~500)
     ! [  276.587542] Bad rss-counter state mm:0000000042cd1e5c type:MM_ANONPAGES
     * Possibly because we aren't handling swapping behaviour or we need some 
     * other way to get if the mm is valid or not
     */
    int n = np * PAGE_SIZE; // 10 pages
    int pid;
    
    if(num == 1){
        // sfork_file
        pid = sfork_file((n + 1)* sizeof(char), CHILD_WRITE | PARENT_WRITE , &ptr); // 1 GB
    }else if(num == 2){
        // sfork_populate
        pid = sfork((n + 1)* sizeof(char), CHILD_WRITE | PARENT_WRITE | SFORK_POPULATE , &ptr); // 1 GB
    }else if(num == 3){
        // sfork
        pid = sfork((n + 1)* sizeof(char), CHILD_WRITE | PARENT_WRITE, &ptr); // 1 GB
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
        char *arr = (char *) ptr;
        for(int i = 0; i < np; i+=2)
            arr[i * PAGE_SIZE] = 100;
        // sleep(5);
    }
    else
    {
        // Child process
        printf("Child PID %d\n", getpid());
        char *arr = (char *) ptr;
        for(int i = 1; i < np; i+=2)
            arr[i * PAGE_SIZE] = 100;
        // wait(NULL);
    }
    return 0;
}
