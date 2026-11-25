#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/models.h"
#include "../include/player.h"
Player player;
void print_team(void)
{
    for (int i = 0; i < player.pokemonTeam.size; i++)
    {
        printf("%d) %s | Nivel: %i | HP: %i\n",
               i + 1,
               player.pokemonTeam.pokemons[i].name,
               player.pokemonTeam.pokemons[i].lvl,
               player.pokemonTeam.pokemons[i].hp);
    }
}

void print_backpack(void)
{
    for (int i = 0; i < player.backpack.itemQuantity; i++)
    {
        printf("%d) %s (%ix)\n",
               i + 1,
               player.backpack.items[i].name,
               player.backpack.items[i].quantity);
    }
}

void add_item(char name[50], int quantity)
{
    for (int i = 0; i < player.backpack.itemQuantity; i++)
    {
        if (strcmp(player.backpack.items[i].name, name) == 0)
        {
            player.backpack.items[i].quantity += quantity;
            return;
        }
    }

    if (player.backpack.itemQuantity < 5)
    {
        int idx = player.backpack.itemQuantity;
        strcpy(player.backpack.items[idx].name, name);
        player.backpack.items[idx].quantity = quantity;
        player.backpack.items[idx].health = 25;
        player.backpack.itemQuantity++;
        return;
    }

    printf("\nUnable to add item...\n");
}

void add_pokemon(Pokemon pokemon)
{
    if (player.pokemonTeam.size < 6)
    {
        player.pokemonTeam.pokemons[player.pokemonTeam.size] = pokemon;
        player.pokemonTeam.size++;
        return;
    }
    printf("\nUnable to add pokemon to team...\n");
}

void select_fighter(char name[50])
{
    for (int i = 0; i < player.pokemonTeam.size; i++)
    {
        if (strcmp(player.pokemonTeam.pokemons[i].name, name) == 0)
        {
            player.pokemonTeam.fighter = player.pokemonTeam.pokemons[i];
            return;
        }
    }
    printf("\nUnable to find %s, error.", name);
}

void use_potion(char name[50])
{
    for (int i = 0; i < player.backpack.itemQuantity; i++)
    {
        if (strcmp(player.backpack.items[i].name, name) == 0)
        {
            player.pokemonTeam.fighter.hp += player.backpack.items[i].health;
            player.backpack.items[i].quantity--;

            if (player.backpack.items[i].quantity <= 0)
            {
                for (int j = i; j < player.backpack.itemQuantity - 1; j++)
                {
                    player.backpack.items[j] = player.backpack.items[j + 1];
                }
                player.backpack.itemQuantity--;
            }
            return;
        }
    }
    printf("\nUnable to find %s, error.", name);
}