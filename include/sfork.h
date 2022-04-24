#ifndef SFORK_H_
#define SFORK_H_

#include <sys/types.h>

#define CHILD_WRITE 0x1
#define PARENT_WRITE 0x2
#define SFORK_POPULATE 0x4
#define MAP_SFORK 	0x200

int sfork_file(size_t len, unsigned int flags, void **paddr);

int sfork(size_t len, unsigned int flags, void **addr);

#endif
