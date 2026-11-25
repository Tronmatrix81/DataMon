#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/models.h"
#include "../include/player.h"
#include "../include/storage.h"

#define MAX_POKEMONES 1038

void limpiar_buffer(void)
{
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

int leer_especies(Species lista[])
{
    FILE *archivo = fopen("../data/pokemons.txt", "rt");
    int total = 0;

    if (archivo == NULL)
    {
        return 0;
    }

    while (total < MAX_POKEMONES &&
           fscanf(archivo, "%s %s %s %s %s %s",
                  lista[total].name,
                  lista[total].type,
                  lista[total].movements[0].movimiento,
                  lista[total].movements[1].movimiento,
                  lista[total].movements[2].movimiento,
                  lista[total].movements[3].movimiento) == 6)
    {
        total++;
    }

    fclose(archivo);
    return total;
}

Pokemon generar_salvaje(Species lista[], int total)
{
    Pokemon salvaje;
    int idx = rand() % total;

    strcpy(salvaje.name, lista[idx].name);
    strcpy(salvaje.type, lista[idx].type);
    salvaje.lvl = 1 + rand() % 50;
    salvaje.hp = 20 + salvaje.lvl * 2;

    for (int i = 0; i < 4; i++)
    {
        strcpy(salvaje.movements[i].movimiento, lista[idx].movements[i].movimiento);
        salvaje.movements[i].daño = 0;
    }

    asociardañomovimientos(&salvaje);
    return salvaje;
}

int luchar_salvaje(int vida)
{
    int dano = 10 + rand() % 16;
    vida -= dano;
    if (vida < 0)
    {
        vida = 0;
    }

    printf("\nLe hiciste %d de daño. Vida restante: %d\n", dano, vida);
    return vida;
}

void capturar_pokemon(Pokemon salvaje)
{
    salvaje.hp = 20 + salvaje.lvl * 2;
    add_pokemon(salvaje);
    printf("\n¡Capturaste a %s nivel %d y se agregó a tu equipo!\n", salvaje.name, salvaje.lvl);
}

void mostrar_menu_captura(Pokemon salvaje)
{
    printf("\n--- %s [%s] (Nivel %d) | Vida: %d ---\n", salvaje.name, salvaje.type, salvaje.lvl, salvaje.hp);
    printf("1. Luchar\n");
    if (salvaje.hp > 0)
    {
        printf("2. Capturar (bloqueado: primero debes dejar su vida en 0)\n");
    }
    else
    {
        printf("2. Capturar\n");
    }
    printf("3. Huir\n");
    printf("Elige una opción: ");
}

void buscar_pokemon(void)
{
    static Species especies[MAX_POKEMONES];
    static int total = 0;
    static int semilla_lista = 0;
    int opcion;
    int terminar = 0;

    if (semilla_lista == 0)
    {
        srand(time(NULL));
        semilla_lista = 1;
    }

    if (total == 0)
    {
        total = leer_especies(especies);
        if (total == 0)
        {
            printf("No se pudo leer ../data/pokemons.txt.\n");
            return;
        }
    }

    Pokemon salvaje = generar_salvaje(especies, total);
    printf("\n¡Apareció un %s (%s) nivel %d!\n", salvaje.name, salvaje.type, salvaje.lvl);

    while (terminar == 0)
    {
        mostrar_menu_captura(salvaje);

        if (scanf("%d", &opcion) != 1)
        {
            limpiar_buffer();
            opcion = 0;
        }

        if (opcion == 1)
        {
            if (salvaje.hp > 0)
            {
                salvaje.hp = luchar_salvaje(salvaje.hp);
                if (salvaje.hp == 0)
                {
                    printf("¡Debilitaste a %s! Ya puedes capturarlo.\n", salvaje.name);
                }
            }
            else
            {
                printf("\n%s ya está debilitado, ¡captúralo!\n", salvaje.name);
            }
        }
        else if (opcion == 2)
        {
            if (salvaje.hp > 0)
            {
                printf("\nNo puedes capturarlo todavía, primero debes luchar hasta que su vida sea 0.\n");
            }
            else
            {
                capturar_pokemon(salvaje);
                terminar = 1;
            }
        }
        else if (opcion == 3)
        {
            printf("\nHuiste del combate.\n");
            terminar = 1;
        }
        else
        {
            printf("\nOpción no válida.\n");
        }   
    }
}