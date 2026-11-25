#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/storage.h"
#include "../include/player.h"
#include "../include/game.h"
#include "../include/ui.h"
#include "../include/combat.h"

void leermovimientos(void);
void asociardañomovimientos(Pokemon *poke);

int main(void) {
    int hayPartidas;
    srand(time(NULL));

    leermovimientos();

    while (1) {
        hayPartidas = hasSavedGames();

        if (hayPartidas) {
            const char *opciones[] = {"Cargar juego", "Nuevo juego", "Ranking", "Salir"};
            int opt = seleccionarMenu("DATAMON - MENU PRINCIPAL", opciones, 4);

            if (opt == 0) {
                printf("Ingrese el nombre de su entrenador: ");
                fgets(player.name, sizeof(player.name), stdin);
                player.name[strcspn(player.name, "\n")] = '\0';

                player.pokemonTeam.size = 0;
                loadGame(&player);

                if (player.pokemonTeam.size > 0) {
                    for (int i = 0; i < player.pokemonTeam.size; i++) {
                        asociardañomovimientos(&player.pokemonTeam.pokemons[i]);
                    }
                    player.pokemonTeam.fighter = player.pokemonTeam.pokemons[0];
                    gameMenu(&player);
                } else {
                    printf("\nPresione Enter para continuar...");
                    getchar();
                }
            }
            else if (opt == 1) {
                newGame();
                gameMenu(&player);
            }
            else if (opt == 2) {
                readStats();
                printf("\nPresione Enter para continuar...");
                getchar();
            }
            else if (opt == 3) {
                return 0;
            }
        } else {
            const char *opciones[] = {"Nuevo juego", "Ranking", "Salir"};
            int opt = seleccionarMenu("DATAMON - MENU PRINCIPAL", opciones, 3);

            if (opt == 0) {
                newGame();
                gameMenu(&player);
            }
            else if (opt == 1) {
                readStats();
                printf("\nPresione Enter para continuar...");
                getchar();
            }
            else if (opt == 2) {
                return 0;
            }
        }
    }
}