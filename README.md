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

## Third-party code
The project uses some open-source libraries to avoid reinventing the wheel and focus on more 'interesting' features. In the future this may be revisited and some of the code may be implemented as part of the project. In particular the heap module is a candidate for this. The following open-source code has been used:

- EmbeddedArtistry libmemory - [malloc_freelist heap memory management](https://github.com/embeddedartistry/libmemory)
- EmbeddedArtistry embedded-resources - [Linked list](https://github.com/embeddedartistry/embedded-resources)
- mpaland/printf - [printf](https://github.com/mpaland/printf)

## Current TODOs

* UART interrupt handling

* Multithreading
