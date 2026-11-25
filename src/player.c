#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/models.h"
#include "../include/player.h"

void print_team()
{
    for (int i = 0; i < 6; i++)
        if (player.pokemonTeam.pokemons[i] != NULL)
            printf("%s, %i, %i\n", player.pokemonTeam.pokemons[i].name, player.pokemonTeam.[i].lvl, player.pokemonTeam.[i].hp);
}

void print_backpack()
{
    for (int i = 0; i < backpack->itemQuantity; i++)
        if (player.backpack.items[i] != NULL)
            printf("%s (%ix)\n", player.backpack.items[i].name, player.backpack.items[i].quantity);
}

void add_item(char name[50], int quantity)
{
    if (player.backpack.itemQuantity < 5)
    {
        for (int i = 0; i < player.backpack.itemQuantity; i++)
        {
            if (strcmp(player.backpack.items[i].name, name) == 0)
            {
                player.backpack.items[i].quantity++;
                return;
            }
        }
        Items item = (Items *)malloc(sizeof(Items));
        if (item != NULL)
        {
            strcpy(item.name, name);
            item.quantity = quantity;
            player.backpack.itemQuantity++;
            player.backpack.items[player.backpack.itemQuantity] = item;
            return;
        }
    }
    printf("\nUnable to add item...\n");
}

void add_pokemon(Pokemon pokemon)
{
    if (player.pokemonTeam.size < 6)
    {
        player.pokemonTeam.size++;
        player.pokemonTeam.pokemons[player.pokemonTeam.size] = pokemon;
        return;
    }
    printf("\nUnable to add pokemon to team...\n");
}

void select_fighter(char name[50])
{
    for (int i = 0; i < 6; i++)
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
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(backpack->items[i].name, name) == 0)
        {
            player.pokemonTeam.fighter.hp += player.backpack.items[i].health;
            player.backpack.itemQuantity--;
            free(player.backpack.items[i]);
            return;
        }
    }
    printf("\nUnable to find %s, error.", name);
}