#ifndef COMBAT_H
#define COMBAT_H

int fhash(char cadena[]);
void leermovimientos();
void asociardañomovimientos(Pokemon *poke);
PokemonTeam random_enemyteam();
void takeDamage(Pokemon *pokemon, int amount);
void arena();

#endif