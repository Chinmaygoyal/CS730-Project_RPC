#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sfork.h>

/*
    Function to perform sfork with file backed mappings.
    size: The size of the shared memory between child and parent
    direction: Unidirectional or Bidirectional
*/
int sfork_file(size_t len, unsigned int flags, void **paddr)
{
    /*
    Call fork from parent and then initialize the memory of the child and the parent
    with the same pfn pages.
    */
    int fd, pid;
    void *addr;
    
    if(len <= 0)
        return -1;
    if(!paddr)
        return -1;
    
    fd = open("temp", O_RDWR|O_CREAT|O_TRUNC, 0644);
    if(fd < 0)
    {
        perror("open error");
        return -1;
    }
    /* 
    Delete the temporary file directory entry so that it can be released. 
    This file is not necessary to exist. It is only used to complete the mapping 
    area for communication between parent and child processes, so unlink.
    */
    unlink("temp");
    ftruncate(fd, len);
    addr = (int *)mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED)
    {
        perror("mmap error");
        return -1;
    }
    close(fd);  //  After the mapping area is established, you can close the file
    *paddr = addr;

    pid = fork();

    /*
    * change the protections for parent and child processes
    */

    if(pid != 0){
        // parent process
        int ret;
        if(flags & PARENT_WRITE)
            ret = mprotect(addr,len,PROT_WRITE | PROT_READ);
        else
            ret = mprotect(addr,len,PROT_READ);
        if(ret == -1) return ret;
    }else{
        // child process
        int ret;
        if(flags & CHILD_WRITE)
            ret = mprotect(addr,len,PROT_WRITE | PROT_READ);
        else
            ret = mprotect(addr,len,PROT_READ);
        if(ret == -1) return ret;
    }

    return pid;
}

/*
    Main function to perform sfork.
    size: The size of the shared memory between child and parent
    direction: Unidirectional or Bidirectional
*/

int sfork(size_t len, unsigned int flags, void **paddr)
{
    if(len <= 0)
        return -1;
    if(!paddr)
        return -1;
    if(!flags){
        printf("Atleast one of parent or child should have write permission\n");
        return -1;
    }
    void *addr;
    
    if(flags & SFORK_POPULATE){
        addr = mmap(NULL,len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SFORK | MAP_POPULATE, -1, 0);
    }else{
        addr = mmap(NULL,len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SFORK, -1, 0);
    }

    *paddr = addr;

    int pid = fork();
    
    if(pid != 0){
        // parent process
        int ret;
        if(flags & PARENT_WRITE)
            ret = mprotect(addr,len,PROT_WRITE | PROT_READ);
        else
            ret = mprotect(addr,len,PROT_READ);
        if(ret == -1) return ret;
    }else{
        // child process
        int ret;
        if(flags & CHILD_WRITE)
            ret = mprotect(addr,len,PROT_WRITE | PROT_READ);
        else
            ret = mprotect(addr,len,PROT_READ);
        if(ret == -1) return ret;
    }
    return pid;
}
