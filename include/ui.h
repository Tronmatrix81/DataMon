#ifndef UI_H
#define UI_H

// Colores ANSI
#define RESET   "\033[0m"
#define NEGRITA "\033[1m"
#define CYAN    "\033[36m"
#define VERDE   "\033[32m"
#define AMARILLO "\033[33m"
#define ROJO    "\033[31m"
#define FONDO_AZUL "\033[44;37m"

// Muestra un menú navegable con flechas (o W/S) y devuelve el índice elegido (0 a n-1)
int seleccionarMenu(const char *titulo, const char *opciones[], int totalOpciones);

// Barra visual de vida para equipo y combates
void imprimirBarraHP(int hpActual, int hpMax);

#endif