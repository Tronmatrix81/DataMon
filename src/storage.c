#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "models.h"
#include "utils.h"
#include "game.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
#endif

char buffer[1024];

int hasSavedGames() {
    #ifdef _WIN32
        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA("saves\\*", &findData);
        if (hFind == INVALID_HANDLE_VALUE) return 0;

        do {
            // Ignore ".", ".."
            if (findData.cFileName[0] != '.') {
                FindClose(hFind);
                return 1;
            }
        } while (FindNextFileA(hFind, &findData) != 0);

        FindClose(hFind);
        return 0;
    #else
        DIR *dir = opendir("saves");
        if (dir == NULL) return 0;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {
                closedir(dir);
                return 1;
            }
        }

        closedir(dir);
        return 0;
    #endif
}

void saveGame(Player player) {
    printf("Guardando el juego...\n");

    updateRanking(&player); // Actualiza el ranking antes de guardar el juego

    char filePath[100];
    snprintf(filePath, sizeof(filePath), "saves/%s.txt", player.name);
    FILE *save = fopen(filePath, "wt");

    if (save != NULL)
    {
        fprintf(save, "%s\n", player.name);
        fprintf(save, "EQUIPO %d\n", player.pokemonTeam.size);

        for (int i = 0; i < player.pokemonTeam.size; i++) fprintf(save, "- %s %d %d\n", player.pokemonTeam.pokemons[i].name, player.pokemonTeam.pokemons[i].lvl, player.pokemonTeam.pokemons[i].hp);

        fprintf(save, "MOCHILA %d\n", player.backpack.itemQuantity);
        
        for (int i = 0; i < player.backpack.itemQuantity; i++) fprintf(save, "- %s %d\n", player.backpack.items[i].name, player.backpack.items[i].quantity);

        fclose(save);
    }
}

void loadGame(Player *player) {
    printf("Cargando el juego...\n");

    char filePath[100];
    snprintf(filePath, sizeof(filePath), "saves/%s.txt", player->name);
    FILE *save = fopen(filePath, "rt");

    if (save == NULL)
    {
        printf("Error al cargar partida...");
        return;
    }

    fscanf(save, "%s", player->name);
    fscanf(save, "EQUIPO %d", &player->pokemonTeam.size);
    for (int i = 0; i < player->pokemonTeam.size; i++)
    {
        fscanf(save, "- %s %d %d",
               player->pokemonTeam.pokemons[i].name,
               &player->pokemonTeam.pokemons[i].lvl,
               &player->pokemonTeam.pokemons[i].hp);
    }

    fclose(save);
}

void readStats() {
    FILE *arch = fopen("data/ranking.txt", "rt");
    if (arch == NULL) {
        printf("No hay registros en el ranking todavia.\n");
        return;
    }

    Player entry;
    printf("\n=== TABLA DE CLASIFICACION ===\n");
    printf("%-18s %-12s %-14s\n", "Entrenador", "Victorias", "Capturados");
    printf("--------------------------------------------------------\n");

    while (fscanf(arch, "%29s %d %d", entry.name, &entry.wins, &entry.pokemonTeam.size) == 3) {
        printf("%-18s %-12d %-14d\n", entry.name, entry.wins, entry.pokemonTeam.size);
    }

    fclose(arch);
}

void updateRanking(Player *player) {
    Player list[10];
    int count = 0;

    // 1. Leer registros existentes (si el archivo ya existe)
    FILE *arch = fopen("data/ranking.txt", "rt");
    if (arch != NULL) {
        while (count < 10 && fscanf(arch, "%29s %d %d",
               list[count].name, &list[count].wins,
               &list[count].pokemonTeam.size) == 3) {
            count++;
        }
        fclose(arch);
    }

    // 2. Buscar si el entrenador ya existe para actualizarlo
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].name, player->name) == 0) {
            list[i].wins = player->wins;
            list[i].pokemonTeam.size  = player->pokemonTeam.size;
            found = 1;
            break;
        }
    }

    // Si es un jugador nuevo, agregarlo al final
    if (!found && count < 10) {
        strcpy(list[count].name, player->name);
        list[count].wins = player->wins;
        list[count].pokemonTeam.size = player->pokemonTeam.size;
        count++;
    }

    // 3. Ordenar de mayor a menor por victorias (Bubble sort)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (list[j].wins < list[j + 1].wins) {
                Player temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }

    // 4. Sobrescribir ranking.txt con la tabla ya ordenada
    arch = fopen("data/ranking.txt", "wt");
    if (arch == NULL) {
        printf("Error al guardar el ranking.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(arch, "%s %d %d\n",
                list[i].name, list[i].wins, list[i].pokemonTeam.size);
    }

    printf("Ranking actualizado.\n");
    fclose(arch);
}

Pokemon *searchPokemon(const char *name)
{
    FILE *arch = fopen("data/pokemons.txt", "rt");
    Pokemon *pokemon = (Pokemon *)malloc(sizeof(Pokemon));
    if (pokemon == NULL)
        return NULL;
    if (arch != NULL)
    {
        for (int i = 0; i < 1037; i++)
        {
            if (fgets(buffer, sizeof(buffer), arch) != NULL)
            {
                sscanf(buffer, "%s %s %s %s %s %s",
       pokemon->name,
       pokemon->type,
       pokemon->movements[0].movimiento,
       pokemon->movements[1].movimiento,
       pokemon->movements[2].movimiento,
       pokemon->movements[3].movimiento);
                if (strcmp(pokemon->name, name) == 0)
                {
                    pokemon->lvl = random2(1, 100);
                    pokemon->hp = random2(1, 3) * pokemon->lvl;
                    fclose(arch);
                    return pokemon;
                }
            }
        }
        fclose(arch);
    }
    free(pokemon);
    return NULL;
}

Pokemon *randomPokemon()
{
    FILE *arch = fopen("data/pokemons.txt", "rt");
    Pokemon *pokemon = (Pokemon *)malloc(sizeof(Pokemon));
    int random = random2(0, 1037);
    if (pokemon == NULL)
        return NULL;
    if (arch != NULL)
    {
        for (int i = 0; i < random; i++)
        {
            if (fgets(buffer, sizeof(buffer), arch) != NULL)
            {
                if (i == random - 1)
                {
                   sscanf(buffer, "%s %s %s %s %s %s",
       pokemon->name,
       pokemon->type,
       pokemon->movements[0].movimiento,
       pokemon->movements[1].movimiento,
       pokemon->movements[2].movimiento,
       pokemon->movements[3].movimiento);
                    fclose(arch);
                    return pokemon;
                }
            }
        }
        fclose(arch);
    }
    free(pokemon);
    return NULL;
}