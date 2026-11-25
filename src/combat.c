#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/storage.h"
#include "../include/models.h"
#include "../include/utils.h"
#include "../include/player.h"

mov *movimientoshasheados[151];

int fhash(char cadena[])
{
    int hasheo = 0;
    for (int i = 0; i < strlen(cadena); i++)
        hasheo += (unsigned char)cadena[i] * 7;
    return hasheo % 151;
}

void leermovimientos()
{
    int hasheo;
    mov *movimientos[72];
    FILE *arch;

    // Inicializamos la tabla hash en NULL
    for (int i = 0; i < 151; i++)
        movimientoshasheados[i] = NULL;

    arch = fopen("../data/movements.txt", "rt");
    if (arch != NULL)
    {
        for (int i = 0; i < 72; i++)
        {
            // Reservamos memoria para cada movimiento antes de leerlo
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
PokemonTeam enemyTeam = NULL;

PokemonTeam random_enemyteam()
{
    PokemonTeam enemyTeam = (PokemonTeam *)malloc(sizeof(PokemonTeam));
    if (enemyTeam != NULL)
    {
        for (int i = 0; i < 6; i++)
            enemyTeam.pokemons[i] = randomPokemon();
    }
}

void takeDamage(Pokemon *pokemon, int amount)
{
    pokemon->hp -= amount;
}

void arena()
{
    enemyTeam = random_enemyteam();
    int turn = random2(0, 1); // player is 0, enemy is 1
    int opc = -1;
    char[50] choice;
    int enemyChoice = 0;
    while (team.fighter.hp > 0 && enemyTeam.fighter.hp > 0)
    {
        printf("\nTu %s (HP: %d) vs %s enemigo (HP: %d)\n",
               team.fighter.name, team.fighter.hp,
               enemyTeam.fighter.name, enemyTeam.fighter.hp);
        if (turn == 0)
        {
            printf("1) %s \n2) %s \n3) %s \n4) %s \n5) Cambiar Criatura\n6) Usar Pocion\n\nOpc: ",
                   player.pokemonTeam.fighter.movements[0].movimiento,
                   player.pokemonTeam.fighter.movements[1].movimiento,
                   player.pokemonTeam.fighter.movements[2].movimiento,
                   player.pokemonTeam.fighter.movements[3].movimiento);
            scanf("%i", &opc);
            switch (opc)
            {
            case 1:
                takeDamage(&enemyTeam.fighter, player.pokemonTeam.fighter.movements[0].daño);
                break;
            case 2:
                takeDamage(&enemyTeam.fighter, player.pokemonTeam.fighter.movements[1].daño);
                break;
            case 3:
                takeDamage(&enemyTeam.fighter, player.pokemonTeam.fighter.movements[2].daño);
                break;
            case 4:
                takeDamage(&enemyTeam.fighter, player.pokemonTeam.fighter.movements[3].daño);
                break;
            case 5:
                print_team();
                printf("\n\nIngrese nombre del nuevo peleador: ");
                scanf(" %s", choice);
                select_fighter(choice);
                break;
            case 6:
                print_backpack();
                printf("\n\nIngrese nombre de pocion: ");
                scanf(" %s", choice);
                use_potion(choice);
                break;
            }
            turn++;
        }
        else
        {
            enemyChoice = random(0, 3);
            if (enemyChoice != 3)
            {
                takeDamage(team->fighter, 15);
                printf("\nTe hizo 15 de danio...");
            }
            else
            {
                enemyTeam.fighter.hp += 10;
                printf("\nSe tomo una pocion...");
            }
            turn--;
        }
    }

     updateRanking(&player);
}

int main(void)
{
    randomPokemon();
    leermovimientos();
    asociardañomovimientos();

    return 0;
}
