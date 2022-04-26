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

- In order to use the library first install the kernel code that is present in the [github repo](https://github.com/Prashant446/linux-5.10.89) and boot that kernel.
- To build user libraries and tests:
```bash
# in root of repo
make
```

- To run the tests:
```bash
    make
    ./bin/[testname] [type]
    # eg: ./bin/test1 2
    # Type map:
        # 1: "sfork_file",
        # 2: "sfork_populate",
        # 3: "sfork_lazy",
        # 4: "shared_mem"
```


- To run the benchmarks:
```bash
    make
    ./runtest.sh [name] [type] [size]
    # eg: ./runtest.sh random 2 1024
    # Type map:
        # 1: "sfork_file",
        # 2: "sfork_populate",
        # 3: "sfork_lazy",
        # 4: "shared_mem"
    
    # to generate plots:
    python plot.py
```

- Tests are present in `tests` folder
- Benchmark programs are present in `benchmark-tests` folder
- Test and benchmark binaries are generated in `bin/` directory.

## Report
All the reports and the presentations are present in `report` folder. The latex source is present as a zip file in the same folder.
