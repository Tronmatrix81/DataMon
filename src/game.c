#include "../include/models.h"

void healingCenter(Player *player)
{
    const char *opciones[] = {
        "Curar a todo el equipo",
        "Recargar pociones en la mochila",
        "Salir del Centro de Curacion"
    };

    while (1)
    {
        int opc = seleccionarMenu("CENTRO DE CURACION DATAMON", opciones, 3);

        if (opc == 0)
        {
            if (player->pokemonTeam.size == 0)
            {
                printf( "\nNo tienes ningun DataMon en tu equipo todavia.\n" );
            }
            else
            {
                for (int i = 0; i < player->pokemonTeam.size; i++)
                {
                    player->pokemonTeam.pokemons[i].hp = 20 + player->pokemonTeam.pokemons[i].lvl * 2;
                }

                if (player->pokemonTeam.fighter.lvl > 0)
                {
                    player->pokemonTeam.fighter.hp = 20 + player->pokemonTeam.fighter.lvl * 2;
                }

                printf( "\n¡Todos tus DataMon han recuperado su salud al maximo!\n" );
                for (int i = 0; i < player->pokemonTeam.size; i++)
                {
                    printf("- %s (Nivel %d) -> HP: %d\n",
                           player->pokemonTeam.pokemons[i].name,
                           player->pokemonTeam.pokemons[i].lvl,
                           player->pokemonTeam.pokemons[i].hp);
                }
            }
        }
        else if (opc == 1)
        {
            int encontrada = 0;
            for (int i = 0; i < player->backpack.itemQuantity; i++)
            {
                if (strcmp(player->backpack.items[i].name, "healthPotion") == 0)
                {
                    player->backpack.items[i].quantity += 2;
                    encontrada = 1;
                    printf( "\nSe agregaron 2 pociones. Total actual: %d\n" ,
                           player->backpack.items[i].quantity);
                    break;
                }
            }

            if (!encontrada && player->backpack.itemQuantity < 5)
            {
                int idx = player->backpack.itemQuantity;
                strcpy(player->backpack.items[idx].name, "healthPotion");
                player->backpack.items[idx].quantity = 2;
                player->backpack.items[idx].health = 25;
                player->backpack.itemQuantity++;
                printf( "\nRecibiste 2 healthPotion en tu mochila.\n" );
            }
        }
        else if (opc == 2)
        {
            printf( "\nSaliendo del Centro de Curacion...\n" );
            return;
        }

        printf("\nPresione Enter para continuar...");
        getchar();
    }
}