#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "models.h"
#include "utils.h"
#include "player.h"

mov *movimientoshasheados[151];

int fhash(char cadena[])
{
    int hasheo = 0;
    for (int i = 0; i < strlen(cadena); i++)
        hasheo += (unsigned char)cadena[i] * 7;
    return hasheo % 151;
}

void leermovimientos(void)
{
    int hasheo;
    mov *movimientos[72];
    FILE *arch;

    for (int i = 0; i < 151; i++)
        movimientoshasheados[i] = NULL;

    arch = fopen("data/movements.txt", "rt");
    if (arch != NULL)
    {
        for (int i = 0; i < 72; i++)
        {
            movimientos[i] = (mov *)malloc(sizeof(mov));
            fscanf(arch, "%s %i", movimientos[i]->movimiento, &movimientos[i]->daño);

            hasheo = fhash(movimientos[i]->movimiento);
            while (movimientoshasheados[hasheo] != NULL)
            {
                hasheo = (hasheo + 1) % 151;
            }
            movimientoshasheados[hasheo] = movimientos[i];
        }
        fclose(arch);
    }
}

void asociardañomovimientos(Pokemon *poke)
{
    int hash;
    for (int i = 0; i < 4; i++)
    {
        hash = fhash(poke->movements[i].movimiento);
        while (movimientoshasheados[hash] != NULL &&
               strcmp(movimientoshasheados[hash]->movimiento, poke->movements[i].movimiento) != 0)
        {
            hash = (hash + 1) % 151;
        }

        if (movimientoshasheados[hash] != NULL)
        {
            poke->movements[i].daño = movimientoshasheados[hash]->daño * poke->lvl;
        }
    }
}

PokemonTeam enemyTeam;

void takeDamage(Pokemon *pokemon, int amount)
{
    pokemon->hp -= amount;
    if (pokemon->hp < 0)
    {
        pokemon->hp = 0;
    }
}

int has_alive_pokemon(PokemonTeam *team)
{
    for (int i = 0; i < team->size; i++)
    {
        if (team->pokemons[i].hp > 0)
        {
            return 1;
        }
    }
    return 0;
}

PokemonTeam random_enemyteam(void)
{
    PokemonTeam teamTemp = {0};
    teamTemp.size = 3; // Equipo rival de 3 DataMons

    for (int i = 0; i < teamTemp.size; i++)
    {
        Pokemon *p = randomPokemon();
        if (p != NULL)
        {
            p->lvl = random2(3, 15);
            p->hp = p->lvl * 3 + 20;
            asociardañomovimientos(p);
            teamTemp.pokemons[i] = *p;
            free(p);
        }
    }
    teamTemp.fighter = teamTemp.pokemons[0];
    return teamTemp;
}

void arena(void)
{
    enemyTeam = random_enemyteam();
    int enemyIdx = 0;
    int turn = random2(0, 1);
    int opc = -1;
    char choice[50];
    int enemyChoice = 0;

    // Asegurar que el jugador entre con un DataMon vivo
    if (player.pokemonTeam.fighter.hp <= 0)
    {
        for (int i = 0; i < player.pokemonTeam.size; i++)
        {
            if (player.pokemonTeam.pokemons[i].hp > 0)
            {
                player.pokemonTeam.fighter = player.pokemonTeam.pokemons[i];
                break;
            }
        }
    }

    while (has_alive_pokemon(&player.pokemonTeam) && enemyIdx < enemyTeam.size)
    {
        printf("\n=== ARENA (Rival %d/%d) ===\n", enemyIdx + 1, enemyTeam.size);
        printf("Tu %s (Nv.%d | HP: %d) vs %s enemigo (Nv.%d | HP: %d)\n",
               player.pokemonTeam.fighter.name, player.pokemonTeam.fighter.lvl, player.pokemonTeam.fighter.hp,
               enemyTeam.fighter.name, enemyTeam.fighter.lvl, enemyTeam.fighter.hp);

        if (turn == 0)
        {
            printf("1) %s \n2) %s \n3) %s \n4) %s \n5) Cambiar Criatura\n6) Usar Pocion\n\nOpc: ",
                   player.pokemonTeam.fighter.movements[0].movimiento,
                   player.pokemonTeam.fighter.movements[1].movimiento,
                   player.pokemonTeam.fighter.movements[2].movimiento,
                   player.pokemonTeam.fighter.movements[3].movimiento);
            scanf("%i", &opc);
            getchar(); // Limpiar '\n' del buffer

            switch (opc)
            {
            case 1:
            case 2:
            case 3:
            case 4:
                takeDamage(&enemyTeam.fighter, player.pokemonTeam.fighter.movements[opc - 1].daño);
                printf("\n%s uso %s - %s perdio %d HP!\n",
                       player.pokemonTeam.fighter.name,
                       player.pokemonTeam.fighter.movements[opc - 1].movimiento,
                       enemyTeam.fighter.name,
                       player.pokemonTeam.fighter.movements[opc - 1].daño);
                break;
            case 5:
                print_team();
                printf("\nIngrese nombre del nuevo peleador: ");
                scanf("%49s", choice);
                getchar();
                select_fighter(choice);
                break;
            case 6:
                print_backpack();
                printf("\nIngrese nombre de pocion: ");
                scanf("%49s", choice);
                getchar();
                use_potion(choice);
                break;
            default:
                printf("\nOpcion invalida, perdiste el turno.\n");
                break;
            }

            // Verificar si cayó el DataMon enemigo actual
            if (enemyTeam.fighter.hp <= 0)
            {
                printf("\n¡Derrotaste a %s enemigo!\n", enemyTeam.fighter.name);
                enemyTeam.pokemons[enemyIdx] = enemyTeam.fighter;
                enemyIdx++;
                if (enemyIdx < enemyTeam.size)
                {
                    enemyTeam.fighter = enemyTeam.pokemons[enemyIdx];
                    printf("¡El rival envia a %s (Nv.%d)!\n", enemyTeam.fighter.name, enemyTeam.fighter.lvl);
                }
            }
            turn = 1;
        }
        else
        {
            enemyChoice = random2(0, 3);
            if (enemyChoice != 3)
            {
                int dano_enemigo = enemyTeam.fighter.movements[enemyChoice].daño;
                takeDamage(&player.pokemonTeam.fighter, dano_enemigo);
                sync_fighter_to_team();
                printf("\n%s enemigo uso %s y te hizo %d de danio!\n",
                       enemyTeam.fighter.name,
                       enemyTeam.fighter.movements[enemyChoice].movimiento,
                       dano_enemigo);
            }
            else
            {
                enemyTeam.fighter.hp += 10;
                printf("\n%s enemigo uso una pocion (+10 HP)!\n", enemyTeam.fighter.name);
            }

            // Si cayó tu DataMon activo pero aún tienes otros vivos, obliga a cambiar
            if (player.pokemonTeam.fighter.hp <= 0)
            {
                printf("\n¡Tu %s ha sido debilitado!\n", player.pokemonTeam.fighter.name);
                if (has_alive_pokemon(&player.pokemonTeam))
                {
                    while (player.pokemonTeam.fighter.hp <= 0)
                    {
                        print_team();
                        printf("\nElige otro DataMon disponible: ");
                        scanf("%49s", choice);
                        getchar();
                        select_fighter(choice);
                    }
                }
            }
            turn = 0;
        }
    }

    sync_fighter_to_team();

    // Evaluar resultado final del combate
    if (enemyIdx >= enemyTeam.size)
    {
        player.wins++;
        add_item("healthPotion", 1);
        printf("\n¡VICTORIA EN LA ARENA! Ganaste 1 healthPotion.\n");
        updateRanking(&player);
    }
    else
    {
        printf("\nTodos tus DataMons fueron debilitados. Ve al Centro de Curacion.\n");
    }
}