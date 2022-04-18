#ifndef SFORK_H_
#define SFORK_H_

#include <sys/types.h>

#define CHILD_WRITE 1
#define PARENT_WRITE 2

int sfork_file(size_t len, unsigned int flags, void **paddr);

int sfork(size_t len, unsigned int flags, void **addr);

#endif
