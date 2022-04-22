#include<stdio.h>
#include<sfork.h>
#include <unistd.h>
#include <sys/wait.h>

#define PAGE_SIZE 4096

int main()
{
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
    int pid = sfork((n + 1)* sizeof(char), CHILD_WRITE | PARENT_WRITE, &ptr);
    
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
