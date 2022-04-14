# CS730-Project

## Aim 
In this project we plan to implement a variant of the fork where some part of the
memory of the parent can be shared with the child process in a manner such that the
update to that memory region from the parent is visible to the child and vice-a-versa. This
sharing can be either unidirectional, i.e., only the parent can update, and the child can
read, and vice-versa, or bidirectional, i.e., both the parent and child can update and see
the updated values. As we know, it can also be achieved with the help of the shared
memory construct. Hence, we will compare sfork with shared memory to study the
effectiveness.

## Usage
To build user libraries and tests:
```bash
# in root of repo
make
```

Test binaries are generated in `bin/` directory.

To build and load kernel module:
```bash
# inside the Module/ dir
make
sudo insmod $sfork.ko
```
