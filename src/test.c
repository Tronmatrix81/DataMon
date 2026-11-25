#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../include/storage.h"
#include "../include/models.h"

int main()
{
    Pokemon *aux = randomPokemon();
    printf("%s", aux->type);
    return 0;
}