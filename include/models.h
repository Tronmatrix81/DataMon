#ifndef MODELS_H
#define MODELS_H

typedef struct {
    char name[35];
    int quantity;
    int health;
} Items;

// 1. Viene de data/movements.txt (ej: "Latigazo 3")
typedef struct {
    char movimiento[50];
    int daño;
} mov;

// 2. Viene de data/pokemons.txt (molde de la especie, sin nivel ni HP)
typedef struct {
    char name[40];         // 40 chars por nombres largos y acentos UTF-8
    char type[20];
    mov movements[4]; // Ya guarda nombre + poder cruzados
} Species;

// 3. Instancia real en juego (en tu equipo, salvaje o rival en Arena)
typedef struct {
    char name[40];
    char type[20];
    int lvl;
    int hp;
    mov movements [4]; // Ya guarda nombre + poder cruzados
    // Aquí va el puntero a la lista/estructura de DataMon capturados
} Pokemon;

typedef struct {
    int size;
    Pokemon pokemons[6];
    Pokemon fighter;
} PokemonTeam;

typedef struct {
    int itemQuantity;
    Items items[5];
} Backpack;

// 4. Estado general de la partida (para guardar/cargar)
typedef struct {
    char name[30];
    int wins;
    Backpack backpack;
    PokemonTeam pokemonTeam;
    // Aquí va el puntero a la lista/estructura de DataMon capturados
} Player;

#endif