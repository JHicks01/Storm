#include "cpu.h"
#include "panic.h"
#include "printf.h"

void
panic(const char *error)
{
    printf("Panic: %s\n", error);

    cpu_halt();
}
