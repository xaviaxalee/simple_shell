#include "shell.h"

/**
 * freeAndNull - frees a pointer and NULLs the address
 * @address: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int freeAndNull(void **address)
{
    if (address && *address)
    {
        free(*address);
        *address = NULL;
        return 1;
    }
    return 0;
}

