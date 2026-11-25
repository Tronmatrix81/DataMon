#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "storage.h"
#include "player.h"
#include "game.h"

int main(void) {
    int hayPartidas;
    srand(time(NULL));
    team = (team*)malloc(sizeof(team)); //asignamos equipo

    while(1) {
        hayPartidas = hasSavedGames();

        if (hayPartidas) {
            const char *opciones[] = {"Cargar juego", "Nuevo juego", "Ranking", "Salir"};
            int opt = seleccionarMenu("DATAMON - MENU PRINCIPAL", opciones, 4);

            if (opt == 0) loadGame();
            else if (opt == 1) newGame();
            else if (opt == 2) readRanking();
            else if (opt == 3) return 0;
        } else {
            const char *opciones[] = {"Nuevo juego", "Ranking", "Salir"};
            int opt = seleccionarMenu("DATAMON - MENU PRINCIPAL", opciones, 3);

            if (opt == 0) newGame();
            else if (opt == 1) readRanking();
            else if (opt == 2) return 0;
        }
    }
}