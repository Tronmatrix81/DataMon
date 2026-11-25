#include "models.h"

extern Player player;
void print_team();
void print_backpack();
void add_item(char name[50], int quantity);
void add_pokemon(Pokemon pokemon);
void sync_fighter_to_team(void);
int use_potion_on(const char *potionName, Pokemon *target);
void select_fighter(char nombre[50]);
void use_potion(char name[50]);