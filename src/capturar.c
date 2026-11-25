#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/models.h"
#include "../include/player.h"
#include "../include/storage.h"

void asociardañomovimientos(Pokemon *poke);

void limpiar_buffer(void)
{
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

Pokemon generar_salvaje(void)
{
    Pokemon salvaje = {0};
    Pokemon *p = randomPokemon();

    if (p == NULL)
    {
        return salvaje; // lvl == 0 indica error de carga
    }

    salvaje = *p;
    free(p);

    salvaje.lvl = 1 + rand() % 35;
    salvaje.hp = salvaje.lvl * 3 + 20;
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
    if (player.pokemonTeam.size >= 6)
    {
        printf("\nTu equipo ya esta lleno (6/6), no puedes capturar mas DataMons.\n");
        return;
    }

    salvaje.hp = salvaje.lvl * 3 + 20;
    add_pokemon(salvaje);
    printf("\n¡Capturaste a %s nivel %d y se agrego a tu equipo!\n", salvaje.name, salvaje.lvl);
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
    printf("Elige una opcion: ");
}

void buscar_pokemon(void)
{
    int opcion;
    int terminar = 0;

    Pokemon salvaje = generar_salvaje();
    if (salvaje.lvl == 0)
    {
        printf("No se pudo generar un DataMon salvaje (revisa data/pokemons.txt).\n");
        return;
    }

    printf("\n¡Aparecio un %s (%s) nivel %d!\n", salvaje.name, salvaje.type, salvaje.lvl);

    while (terminar == 0)
    {
        mostrar_menu_captura(salvaje);

        if (scanf("%d", &opcion) != 1)
        {
            opcion = 0;
        }
        limpiar_buffer(); // Evita que quede '\n' atorado al volver a gameMenu()

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
                printf("\n%s ya esta debilitado, ¡capturalo!\n", salvaje.name);
            }
        }
        else if (opcion == 2)
        {
            if (salvaje.hp > 0)
            {
                printf("\nNo puedes capturarlo todavia, primero debes luchar hasta que su vida sea 0.\n");
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
            printf("\nOpcion no valida.\n");
        }
    }
}