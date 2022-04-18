#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAP_SFORK 	0x200

struct pid_node{
    pid_t pid;
    struct pid_node* next;
};

struct meta_page{
    int ref_count;
    struct pid_node* head;
};

void add_to_last(struct meta_page* mp, pid_t pid){
    struct pid_node* cnode = mp->head;
    if(!cnode){
        mp->head = malloc(sizeof(struct pid_node));
        mp->head->pid = pid;
        mp->head->next = NULL;
    }else{
        while(cnode->next != NULL) cnode = cnode->next;
        cnode->next = malloc(sizeof(struct pid_node));
        cnode = cnode->next;
        cnode->pid = pid;
        cnode->next = NULL;
    }
    return;
}

int create_handle()
{
    int fd = open("/dev/sfork_dev", O_RDWR);
    return fd;
}

void close_handle(int cdev)
{
    close(cdev);
    return;
}

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

    return pid;
}

/*
    Main function to perform sfork.
    size: The size of the shared memory between child and parent
    direction: Unidirectional or Bidirectional
*/

int sfork(size_t len, unsigned int flags, void **paddr)
{
    if(!flags){
        printf("Atleast one of parent or child should have write permission\n");
        return -1;
    }

    len += 4096;

    void* addr = mmap(NULL,len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SFORK | MAP_POPULATE, -1, 0);
    printf("%p\n",addr);
    *paddr = addr + 4096;
    int direction = flags;
    
    struct meta_page mp;
    mp.ref_count = 2;
    // add_to_last(&mp,getpid());
    // *(struct  meta_page*)addr = mp;

    *(int*)addr = 5;
    
    int pid = fork();
    /* First give write permissions to the correct process
    Then write 2 and the corresponding process ids to the first page
    */

    return pid;

    if(pid == 0){
        // child process
        printf("In the child process\n");
        int value = *(int *)addr;
        printf("Value from child_1:%d\n",value);

        *(int*)addr = 10;
        printf("Value from child_2:%d\n",*(int *)addr);

        // if(direction & 1){
        //     //child has the write permissions
        //     printf("Changing the permission for the child\n");
        //     mprotect(addr,len,PROT_READ | PROT_WRITE);
        // }
        
        // if((direction & 1) && !(direction & 2)){
        //     // only child has the write permissions
        //     add_to_last(&mp,getppid());
        //     add_to_last(&mp,getppid());    
        //     *(struct  meta_page*)addr = mp;
        //     // allocate the same page to parent
        // }
        // sleep(1);

    }else{
        printf("In the parent process\n");
        wait(NULL);
        
        int value = *(int *)addr;
        printf("Value from parent_1:%d\n",value);
        
        // if(direction & 2){
        //     // parent has the write permission
        //     printf("Changing the permission for the parent\n");
        //     mprotect(addr,len,PROT_READ | PROT_WRITE);
        //     add_to_last(&mp,getpid());
        //     add_to_last(&mp,pid);
        //     *(struct  meta_page*)addr = mp;
        //     struct meta_page temp = *(struct  meta_page*)addr;
        //     printf("Ref_count: %d\n",temp.ref_count);
        //     printf("PID_parent_metapage: %d\n",temp.head->pid);
        //     printf("PID_child_metapage: %d\n",temp.head->next->pid);
        //     printf("Original PID:%d\n",getpid());
        //     printf("Original CHILD_PID:%d\n",pid);
        //     // allocate the same page to child

        //     // virtual address
        //     // child pid
        //     int ret = write(create_handle(),&(addr),pid);
        // }
        // parent proces
    }
    return pid;
}
