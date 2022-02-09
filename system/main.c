/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

mutex_t	lock = FALSE;
void    printchar(char c)
{
    int i;
    mutex_lock(&lock);
    for (i=0; i<10; i++)
        kprintf("%c", c);
    mutex_unlock(&lock);
}

int main(uint32 argc, uint32 *argv)
{
    //priority of process is input as the 3rd argument of create()
    ready(create((void*) printchar, INITSTK, 15, "P1", 1, 'A'), FALSE);
    ready(create((void*) printchar, INITSTK, 15, "P2", 1, 'B'), FALSE);

    return 0;
}
