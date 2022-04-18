#include<stdio.h>
#include<sfork.h>
#include <sys/wait.h>

int main()
{
    void *ptr;
    int n = 101;
    int pid = sfork((n+1)* sizeof(int), CHILD_WRITE | PARENT_WRITE, &ptr);
    if(pid < 0)
    {
        printf("fork failed!\n");
        return -1;
    }
    else if(pid)
    {
        // wait(NULL);
        int *arr = (int *) ptr;
        arr[n] = 0;
        while(arr[n] == 0);
        printf("Parent: Computing sum\n");
        int sum = 0;
        for(int i = 0; i < n; i++)
            sum += arr[i];
        arr[0] = sum;
        arr[n] = 0;
        printf("parent addr = %p\n", ptr);
    }
    else
    {
        // Child process
        int *arr = (int *) ptr;
        printf("Child: Generating array\n");
        for(int i = 0; i < n; i++)
            arr[i] = i;
        arr[n] = 1;
        while(arr[n] == 1);
        printf("Child: Sum of numbers: %d\n", arr[0]);
        printf("child addr = %p\n", ptr);
    }

    return 0;
}
