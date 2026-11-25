#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define CLEAR system("cls")
#else
#define CLEAR system("clear")
#endif

int main(void) {
    int opt;

    while(1) {
        CLEAR;
        printf("=== DataMon ===\n");
        printf("1. Nuevo juego\n");
        printf("2. Cargar juego\n");
        printf("3. Ranking\n");
        printf("0. Salir\n");
        printf("\n-> ");
        scanf("%d", &opt);

        switch (opt) {
        case 0:
            return 0;

        case 1:
            CLEAR;
            //newGame();
            break;

        case 2:
            CLEAR;
            //loadGame();
            break;

        case 3:
            CLEAR;
            //stats();
            break;
        
        default:
            printf("Opcion invalida\n");
            break;
        }

        printf("\nPresione Enter para continuar...");
        getchar(); // Limpiar el buffer de entrada
        getchar(); // Esperar a que el usuario presione Enter antes de continuar
    }
}