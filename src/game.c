#include <stdio.h>
#include <string.h>
#include "game.h"
#include "ui.h"
#include "storage.h"
#include "utils.h"
#include "combat.h"

void newGame() {
    Player player;
    
    printf("DATAMON - NUEVO JUEGO\n");
    printf("Ingrese el nombre de su entrenador: ");
    fgets(player.name, sizeof(player.name), stdin);
    player.name[strcspn(player.name, "\n")] = '\0'; // Eliminar el salto de línea
    player.wins = 0;
    player.pokemonTeam.size = 0;
    player.backpack.itemQuantity = 1;
    strcat(player.backpack.items[0].name, "healthPotion");
    player.backpack.items[0].quantity = 1;
    player.backpack.items[0].health = random2(1, 30);
}

void gameMenu(Player *player) {
    const char *opciones[] = {
        "Curar a todo el equipo",
        "Recargar pociones en la mochila",
        "Salir del Centro de Curacion"
    };

    while (1) {
        int opc = seleccionarMenu("DATAMON - EN PARTIDA", opciones, 7);

        switch (opc) {
        case 0: // 1) Ver equipo
            // showTeam(player);
            printf(CYAN "\nMostrando equipo de %s...\n" RESET, player->name);
            break;

        case 1: // 2) Buscar DataMon salvaje
            // wildEncounter(player);
            printf(VERDE "\nBuscando DataMon salvaje en la hierba...\n" RESET);
            break;

        case 2: // 3) Ir a Arena
            // startArena(player);
            printf(ROJO "\nEntrando a la Arena de combate...\n" RESET);
            break;

        case 3: // 4) Ver Mochila
            // showBag(player);
            printf(AMARILLO "\nAbriendo mochila...\n" RESET);
            break;

        case 4: // 5) Centro de curacion
            // healingCenter(player);
            printf(VERDE "\nBienvenido al Centro de Curacion.\n" RESET);
            break;

        case 5: // 6) Guardar partida
            saveGame(*player);
            break;

        case 6: // 7) Volver al menu principal
            return; // Sale del while(1) y regresa a newGame() / loadGame()
        }

        printf("\nPresione Enter para continuar...");
        getchar();
    }
}