#include <stdlib.h>

int random2(int min, int max)
{
    int rand1 = rand();
    return rand1 % (max-min+1) + min;
}