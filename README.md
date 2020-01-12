# Storm
A small kernel for i386 systems.

## How to build
Follow [OSDev wiki instructions](https://wiki.osdev.org/GCC_Cross-Compiler#The_Build) to build and install a cross compiler and then run:

```
$ export CROSS_COMPILER_PATH=/path/to/i686-elf-gcc
$ make
```

## Running on QEMU
Install i386 QEMU and then run:

```
$ make run_qemu
```

## Current TODOs

* UART interrupt handling

* Heap memory management

* Multithreading
