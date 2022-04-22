#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>


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

void print_metapage(struct meta_page* mp){
    printf("The ref count is: %d\n", mp->ref_count);
    struct pid_node* c_node = mp->head;
    while(c_node){
        printf("The pid: %d\n",c_node->pid);
        c_node = c_node->next;
    }
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

    void* addr = mmap(NULL,len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SFORK, -1, 0);
    *paddr = addr;
    int direction = flags;
    
    printf("%px\n",addr);
    int fd = open("/dev/sfork_dev", O_RDWR);
    write(fd,&addr,8);
    int pid = fork();
    return pid;

    if(pid == 0) {

        printf("Child PID: %d\n",getpid());
        sleep(2);
        write(fd,&addr,8);
        printf("In the child process\n");
        *(int*)addr = 20;
        printf("Updated value from child_2:%d\n",*(int *)addr);
    } else {
        wait(NULL);
        write(fd,&addr,8);
        printf("In the parent process\n");
        int value = *(int *)addr;
        printf("Value from parent_1:%d\n",value);
    }
    return pid;
}
