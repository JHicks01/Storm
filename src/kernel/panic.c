#include "bootstrap_print.h"
#include "cpu.h"
#include "panic.h"

void
panic(const char *error)
{
    print_str("Panic: ");
    print_str(error);
    print_str("\n");

    cpu_halt();
}
