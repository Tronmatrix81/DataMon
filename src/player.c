#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
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

void sync_fighter_to_team(void)
{
    for (int i = 0; i < player.pokemonTeam.size; i++)
    {
        if (strcmp(player.pokemonTeam.pokemons[i].name, player.pokemonTeam.fighter.name) == 0)
        {
            player.pokemonTeam.pokemons[i] = player.pokemonTeam.fighter;
            return;
        }
    }
}

static int iguales_sin_mayusculas(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}

void select_fighter(char name[50])
{
    // 1. Guardar el HP actual del peleador que sale antes de cambiarlo
    sync_fighter_to_team();

    // 2. Acepta el numero de la lista (1, 2, ...) o el nombre (sin importar mayusculas)
    int idx = -1;
    char *fin;
    long n = strtol(name, &fin, 10);
    if (*name != '\0' && *fin == '\0')
    {
        if (n >= 1 && n <= player.pokemonTeam.size) idx = (int)n - 1;
    }
    else
    {
        for (int i = 0; i < player.pokemonTeam.size; i++)
        {
            if (iguales_sin_mayusculas(player.pokemonTeam.pokemons[i].name, name)) { idx = i; break; }
        }
    }

    if (idx < 0)
    {
        printf("\nNo se encontro a %s en tu equipo.\n", name);
        return;
    }
    if (player.pokemonTeam.pokemons[idx].hp <= 0)
    {
        printf("\n%s esta debilitado (0 HP) y no puede pelear.\n", player.pokemonTeam.pokemons[idx].name);
        return;
    }
    player.pokemonTeam.fighter = player.pokemonTeam.pokemons[idx];
    printf("\n¡Adelante, %s!\n", player.pokemonTeam.fighter.name);
}

int use_potion_on(const char *potionName, Pokemon *target)
{
    for (int i = 0; i < player.backpack.itemQuantity; i++)
    {
        if (strcmp(player.backpack.items[i].name, potionName) == 0 &&
            player.backpack.items[i].quantity > 0)
        {
            int maxHp = target->lvl * 3 + 20;
            target->hp += player.backpack.items[i].health;
            if (target->hp > maxHp)
            {
                target->hp = maxHp;
            }

            player.backpack.items[i].quantity--;
            if (player.backpack.items[i].quantity <= 0)
            {
                for (int j = i; j < player.backpack.itemQuantity - 1; j++)
                {
                    player.backpack.items[j] = player.backpack.items[j + 1];
                }
                player.backpack.itemQuantity--;
            }

            // Si el curado era el peleador activo, actualizar también fighter
            if (strcmp(target->name, player.pokemonTeam.fighter.name) == 0)
            {
                player.pokemonTeam.fighter.hp = target->hp;
            }
            return 1;
        }
    }
    return 0;
}

void use_potion(char name[50])
{
    if (!use_potion_on(name, &player.pokemonTeam.fighter))
    {
        printf("\nNo tienes %s en la mochila.\n", name);
    }
}