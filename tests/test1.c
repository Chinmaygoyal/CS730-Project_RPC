#include<stdio.h>
#include<sfork.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    void *ptr;
    int n = 10 * 4096; // 10 pages
    int pid = sfork((n + 1)* sizeof(int), CHILD_WRITE | PARENT_WRITE, &ptr);
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
