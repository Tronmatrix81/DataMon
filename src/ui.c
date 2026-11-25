#include <stdio.h>
#include "ui.h"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEAR system("cls")
#else
    #include <termios.h>
    #include <unistd.h>
    #define CLEAR system("clear")
#endif

enum Tecla { TECLA_ARRIBA = 1, TECLA_ABAJO, TECLA_ENTER, TECLA_OTRA };

static int leerTecla(void) {
#ifdef _WIN32
    // Habilita colores ANSI en consolas clásicas de Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    int ch = _getch();
    if (ch == 0 || ch == 224) { // Prefijo de teclas especiales en Windows
        switch (_getch()) {
            case 72: return TECLA_ARRIBA;
            case 80: return TECLA_ABAJO;
        }
    }
    if (ch == 13) return TECLA_ENTER;
    if (ch == 'w' || ch == 'W') return TECLA_ARRIBA;
    if (ch == 's' || ch == 'S') return TECLA_ABAJO;
    return TECLA_OTRA;
#else
    struct termios orig, raw;
    tcgetattr(STDIN_FILENO, &orig);
    raw = orig;
    raw.c_lflag &= ~(ICANON | ECHO); // Lee tecla al instante sin esperar Enter
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int res = TECLA_OTRA;
    int ch = getchar();
    if (ch == '\033') { // Secuencia de escape de flechas en Linux/macOS
        if (getchar() == '[') {
            switch (getchar()) {
                case 'A': res = TECLA_ARRIBA; break;
                case 'B': res = TECLA_ABAJO;  break;
            }
        }
    } else if (ch == '\n' || ch == '\r') {
        res = TECLA_ENTER;
    } else if (ch == 'w' || ch == 'W') {
        res = TECLA_ARRIBA;
    } else if (ch == 's' || ch == 'S') {
        res = TECLA_ABAJO;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &orig); // Siempre restaurar la terminal
    return res;
#endif
}

int seleccionarMenu(const char *titulo, const char *opciones[], int totalOpciones) {
    int seleccionado = 0;

    while (1) {
        CLEAR;
        printf(CYAN NEGRITA "╔════════════════════════════════════╗\n");
        printf("║  %-34s║\n", titulo);
        printf("╚════════════════════════════════════╝\n\n" RESET);

        for (int i = 0; i < totalOpciones; i++) {
            if (i == seleccionado) {
                printf("  " FONDO_AZUL NEGRITA " ▶ %-30s " RESET "\n", opciones[i]);
            } else {
                printf("     %-30s\n", opciones[i]);
            }
        }

        printf("\n" AMARILLO "[↑/↓ o W/S] Mover   [Enter] Seleccionar" RESET "\n");
        fflush(stdout);

        int tecla = leerTecla();
        if (tecla == TECLA_ARRIBA) {
            seleccionado = (seleccionado - 1 + totalOpciones) % totalOpciones;
        } else if (tecla == TECLA_ABAJO) {
            seleccionado = (seleccionado + 1) % totalOpciones;
        } else if (tecla == TECLA_ENTER) {
            return seleccionado;
        }
    }
}

void imprimirBarraHP(int hpActual, int hpMax) {
    int ancho = 20;
    int llenos = (hpMax > 0) ? (hpActual * ancho) / hpMax : 0;
    const char *color = VERDE;

    if (hpActual <= hpMax * 0.25) color = ROJO;
    else if (hpActual <= hpMax * 0.50) color = AMARILLO;

    printf("HP: %s[", color);
    for (int i = 0; i < ancho; i++) {
        printf("%s", (i < llenos) ? "█" : "░");
    }
    printf("]" RESET " %d/%d\n", hpActual, hpMax);
}