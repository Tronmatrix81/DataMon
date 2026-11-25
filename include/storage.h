#ifndef STORAGE_H
#define STORAGE_H

#include "models.h"

// Aqui vive todo lo que lee/escribe archivos en data/ y saves/
int hasSavedGames();
void saveGame(Player player);
void loadGame(Player *player);
void readRanking();
void readStats(void);
void updateRanking(Player *player);
Pokemon *searchPokemon();
Pokemon *randomPokemon();

#endif