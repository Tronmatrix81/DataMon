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
        printf("Error al cargar partida (no existe %s)...\n", filePath);
        return;
    }

    char name[30];
    int size = 0;
    if (fscanf(save, "%29s", name) != 1 || fscanf(save, " EQUIPO %d", &size) != 1 || size < 0 || size > 6)
    {
        printf("Archivo de partida danado...\n");
        fclose(save);
        return;
    }
    strcpy(player->name, name);
    player->pokemonTeam.size = 0;

    for (int i = 0; i < size; i++)
    {
        char pname[40];
        int lvl, hp;
        if (fscanf(save, " - %39s %d %d", pname, &lvl, &hp) != 3) break;

        // Recupera tipo y movimientos desde data/pokemons.txt
        Pokemon *base = searchPokemon(pname);
        Pokemon *dst = &player->pokemonTeam.pokemons[player->pokemonTeam.size];
        if (base != NULL) { *dst = *base; free(base); }
        else { memset(dst, 0, sizeof(Pokemon)); strcpy(dst->name, pname); }
        dst->lvl = lvl;
        dst->hp = hp;
        player->pokemonTeam.size++;
    }

    player->backpack.itemQuantity = 0;
    int items = 0;
    if (fscanf(save, " MOCHILA %d", &items) == 1)
    {
        for (int i = 0; i < items && player->backpack.itemQuantity < 5; i++)
        {
            Items *it = &player->backpack.items[player->backpack.itemQuantity];
            if (fscanf(save, " - %34s %d", it->name, &it->quantity) != 2) break;
            it->health = 25;
            player->backpack.itemQuantity++;
        }
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

// Lee una linea de data/pokemons.txt y la convierte en Pokemon (sin nivel/HP). Devuelve 1 si OK.
static int parsePokemonLine(const char *line, Pokemon *p)
{
    return sscanf(line, "%39s %19s %49s %49s %49s %49s",
                  p->name, p->type,
                  p->movements[0].movimiento, p->movements[1].movimiento,
                  p->movements[2].movimiento, p->movements[3].movimiento) == 6;
}

Pokemon *searchPokemon(const char *name)
{
    FILE *arch = fopen("data/pokemons.txt", "rt");
    if (arch == NULL) return NULL;

    Pokemon *pokemon = (Pokemon *)calloc(1, sizeof(Pokemon));
    if (pokemon == NULL) { fclose(arch); return NULL; }

    while (fgets(buffer, sizeof(buffer), arch) != NULL)
    {
        if (parsePokemonLine(buffer, pokemon) && strcmp(pokemon->name, name) == 0)
        {
            pokemon->lvl = random2(1, 100);
            pokemon->hp = random2(1, 3) * pokemon->lvl;
            fclose(arch);
            return pokemon;
        }
    }
    fclose(arch);
    free(pokemon);
    return NULL;
}

Pokemon *randomPokemon(void)
{
    FILE *arch = fopen("data/pokemons.txt", "rt");
    if (arch == NULL) return NULL;

    Pokemon *pokemon = (Pokemon *)calloc(1, sizeof(Pokemon));
    if (pokemon == NULL) { fclose(arch); return NULL; }

    // Seleccion uniforme sin conocer el total de lineas (reservorio)
    int count = 0;
    Pokemon temp;
    while (fgets(buffer, sizeof(buffer), arch) != NULL)
    {
        if (parsePokemonLine(buffer, &temp))
        {
            count++;
            if (random2(1, count) == 1) *pokemon = temp;
        }
    }
    fclose(arch);

    if (count == 0) { free(pokemon); return NULL; }
    return pokemon;
}
