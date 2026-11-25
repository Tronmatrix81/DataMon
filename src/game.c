#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/game.h"
#include "../include/ui.h"
#include "../include/storage.h"
#include "../include/utils.h"
#include "../include/combat.h"
#include "../include/player.h"

void asociardañomovimientos(Pokemon *poke);

void newGame(void) {
    printf("DATAMON - NUEVO JUEGO\n");
    printf("Ingrese el nombre de su entrenador: ");
    fgets(player.name, sizeof(player.name), stdin);
    player.name[strcspn(player.name, "\n")] = '\0';

    player.wins = 0;
    player.pokemonTeam.size = 0;
    add_item("healthPotion", 1);
    add_pokemon(*randomPokemon());

    saveGame(player);

    if (player.pokemonTeam.size > 0) {
        printf("\nEnhorabuena! Conseguiste a %s (Nv. %d | HP: %d)\n",
               player.pokemonTeam.pokemons[0].name,
               player.pokemonTeam.pokemons[0].lvl,
               player.pokemonTeam.pokemons[0].hp);
    }
    printf("Conseguiste una pocion de vida! (+%d HP)\n", player.backpack.items[0].health);
    printf("Listo para empezar tu aventura!\n");
    printf("\nPresione Enter para continuar...");
    getchar();

    gameMenu(&player);
}

void gameMenu(Player *p) {
    const char *opciones[] = {
        "Ver equipo",
        "Buscar DataMon salvaje",
        "Ir a Arena",
        "Ver Mochila",
        "Centro de curacion",
        "Guardar partida",
        "Volver al menu principal"
    };

    while (1) {
        int opc = seleccionarMenu("DATAMON - EN PARTIDA", opciones, 7);

        switch (opc) {
        case 0: // 1) Ver equipo
            printf(CYAN "\nMostrando equipo de %s...\n" RESET, p->name);
            print_team();
            break;

        case 1: { // 2) Buscar DataMon salvaje
            printf(VERDE "\nBuscando DataMon salvaje en la hierba...\n" RESET);
            Pokemon *salvaje = randomPokemon();
            if (salvaje != NULL) {
                salvaje->lvl = random2(1, 20);
                salvaje->hp = random2(1, 3) * salvaje->lvl + 15;
                asociardañomovimientos(salvaje);
                printf("Aparecio un %s salvaje (Nv. %d | HP: %d)!\n",
                       salvaje->name, salvaje->lvl, salvaje->hp);

                if (p->pokemonTeam.size < 6) {
                    add_pokemon(*salvaje);
                    printf(VERDE "Capturaste a %s!\n" RESET, salvaje->name);
                } else {
                    printf(AMARILLO "Tu equipo esta lleno (6/6).\n" RESET);
                }
                free(salvaje);
            } else {
                printf(ROJO "No aparecieron DataMons (revisa ../data/pokemons.txt).\n" RESET);
            }
            break;
        }

        case 2: // 3) Ir a Arena
            printf(ROJO "\nEntrando a la Arena de combate...\n" RESET);
            if (p->pokemonTeam.size > 0) {
                if (p->pokemonTeam.fighter.hp <= 0) {
                    p->pokemonTeam.fighter = p->pokemonTeam.pokemons[0];
                }
                arena();
                // Sincronizar HP del peleador activo con su ranura en el equipo
                for (int i = 0; i < p->pokemonTeam.size; i++) {
                    if (strcmp(p->pokemonTeam.pokemons[i].name, p->pokemonTeam.fighter.name) == 0) {
                        p->pokemonTeam.pokemons[i] = p->pokemonTeam.fighter;
                        break;
                    }
                }
            } else {
                printf("No tienes DataMons en tu equipo todavia.\n");
            }
            break;

        case 3: // 4) Ver Mochila
            printf(AMARILLO "\nAbriendo mochila...\n" RESET);
            print_backpack();
            break;

        case 4: // 5) Centro de curacion
            printf(VERDE "\nBienvenido al Centro de Curacion.\n" RESET);
            for (int i = 0; i < p->pokemonTeam.size; i++) {
                p->pokemonTeam.pokemons[i].hp = p->pokemonTeam.pokemons[i].lvl * 3 + 20;
            }
            if (p->pokemonTeam.size > 0) {
                p->pokemonTeam.fighter = p->pokemonTeam.pokemons[0];
            }
            printf("Todo tu equipo ha sido curado al maximo!\n");
            break;

        case 5: // 6) Guardar partida
            saveGame(*p);
            break;

        case 6: // 7) Volver al menu principal
            return;
        }

        printf("\nPresione Enter para continuar...");
        getchar();
    }
}