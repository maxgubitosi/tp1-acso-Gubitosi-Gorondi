#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"


int main()
{
    // test load instruction from memory
    mem_write_32(0x0, 0x20000000);
    return 0;
}
