#include <stdio.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#define key 0x1234
#define PAGE_SIZE (1 << 12)

int main(){
    int fd = open("test_file.txt", O_RDWR | O_CREAT);
    int shm_id = shmget(key, PAGE_SIZE, IPC_CREAT | 0600);
    void* mem = shmat(shm_id, NULL, 0);
    // void* mem = shmat(shm_id, NULL, SHM_RDONLY);
    mmap(mem, PAGE_SIZE*4, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fd, 0);
    // *(int*)mem = 'R'+'P'+'C';
    shmdt(mem);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}
