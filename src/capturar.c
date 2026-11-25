/*Leer los datamones del archivo datamones.txt,
generar uno aleatorio con nivel y vida aleatorios y mostrar un menú(Luchar, Capturar, Huir). La opción Capturar no deja usarse hasta que
la vida del datamon llegue a 0. La función que realmente captura
(guardar en la estructura) la programan mis compañeros*/

/*Para el equipo: llamar a buscar_datamon() desde el menú principal y
programar el cuerpo de capturar_datamon(). datamones.txt = un nombre
por línea*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DATAMONES 1037 /*máximo de nombres que se leen del archivo */
#define LARGO_NOMBRE 50

/* Datamon salvaje que aparece al buscar */
typedef struct {
    char nombre[LARGO_NOMBRE];
    int nivel;
    int vida;
} DatamonSalvaje;


/* FUNCIÓN AGREGADA: borra lo que quede en el teclado si escriben letras */
void limpiar_buffer(void)
{
    int c;

    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}


/* FUNCIÓN AGREGADA: quita el salto de línea que fgets deja al final */
void quitar_salto(char texto[])
{
    int i;

    for (i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == '\n' || texto[i] == '\r') {
            texto[i] = '\0';
        }
    }
}


/* FUNCIÓN AGREGADA: lee los nombres de datamones.txt y regresa cuántos
 * leyó (0 si no encontró el archivo o está vacío) */
int leer_datamones(char nombres[][LARGO_NOMBRE])
{
    FILE *archivo;
    char linea[LARGO_NOMBRE];
    int total;

    total = 0;
    archivo = fopen("datamones.txt", "r");
    if (archivo == NULL) {
        return 0;
    }

    while (total < MAX_DATAMONES && fgets(linea, LARGO_NOMBRE, archivo) != NULL) {
        quitar_salto(linea);
        if (linea[0] != '\0') {          /* se salta las líneas vacías */
            strcpy(nombres[total], linea);
            total++;
        }
    }

    fclose(archivo);
    return total;
}


/* FUNCIÓN AGREGADA: crea el datamon con nombre al azar, nivel de 1 a 50
 * y vida según el nivel (a más nivel, más vida) */
DatamonSalvaje generar_datamon(char nombres[][LARGO_NOMBRE], int total)
{
    DatamonSalvaje datamon;

    strcpy(datamon.nombre, nombres[rand() % total]);
    datamon.nivel = 1 + rand() % 50;
    datamon.vida = 20 + datamon.nivel * 2;

    return datamon;
}


/* FUNCIÓN AGREGADA: ataque con daño al azar (10 a 25). Recibe la vida
 * actual y regresa la nueva, sin bajar de 0 */
int luchar(int vida)
{
    int dano;

    dano = 10 + rand() % 16;
    vida = vida - dano;
    if (vida < 0) {
        vida = 0;
    }

    printf("\nLe hiciste %d de daño. Vida restante: %d\n", dano, vida);
    return vida;
}


/* FUNCIÓN AGREGADA: AQUÍ VA EL CÓDIGO DE MIS COMPAÑEROS. Se llama solo
 * cuando la vida ya es 0 (no hay que volver a validarlo). Por ahora
 * solo muestra un mensaje para probar el menú. */
void capturar_datamon(DatamonSalvaje datamon)
{
    printf("\n¡Capturaste a %s nivel %d!\n", datamon.nombre, datamon.nivel);
    /* TODO (compañeros): guardar el datamon en arreglo/lista/hash/árbol */
}


/* FUNCIÓN AGREGADA: muestra los datos del datamon y el menú.
 * Capturar sale como bloqueada mientras la vida sea mayor a 0 */
void mostrar_menu(DatamonSalvaje datamon)
{
    printf("\n--- %s (nivel %d) | Vida: %d ---\n", datamon.nombre, datamon.nivel, datamon.vida);
    printf("1. Luchar\n");
    if (datamon.vida > 0) {
        printf("2. Capturar (bloqueado: primero debes dejar su vida en 0)\n");
    } else {
        printf("2. Capturar\n");
    }
    printf("3. Huir\n");
    printf("Elige una opción: ");
}


/* FUNCIÓN AGREGADA: es la que se llama desde el menú principal al elegir
 * "Buscar Datamon". Genera el datamon y repite el menú hasta que el
 * jugador capture o huya */
void buscar_datamon(void)
{
    char nombres[MAX_DATAMONES][LARGO_NOMBRE];
    int total;
    int opcion;
    int terminar;
    DatamonSalvaje datamon;
    static int semilla_lista = 0;

    /* La semilla del aleatorio se pone solo la primera vez */
    if (semilla_lista == 0) {
        srand(time(NULL));
        semilla_lista = 1;
    }

    total = leer_datamones(nombres);
    if (total == 0) {
        printf("No se pudo leer datamones.txt (o está vacío).\n");
        return;
    }

    datamon = generar_datamon(nombres, total);
    printf("\n¡Apareció un %s nivel %d!\n", datamon.nombre, datamon.nivel);

    terminar = 0;
    while (terminar == 0) {
        mostrar_menu(datamon);

        if (scanf("%d", &opcion) != 1) {   /* escribieron letras */
            limpiar_buffer();
            opcion = 0;
        }

        if (opcion == 1) {
            /* Luchar */
            if (datamon.vida > 0) {
                datamon.vida = luchar(datamon.vida);
                if (datamon.vida == 0) {
                    printf("¡Debilitaste a %s! Ya puedes capturarlo.\n", datamon.nombre);
                }
            } else {
                printf("\n%s ya está debilitado, ¡captúralo!\n", datamon.nombre);
            }
        } else if (opcion == 2) {
            /* Capturar: solo si la vida es 0 */
            if (datamon.vida > 0) {
                printf("\nNo puedes capturarlo todavía, primero debes luchar hasta que su vida sea 0.\n");
            } else {
                capturar_datamon(datamon);
                terminar = 1;
            }
        } else if (opcion == 3) {
            /* Huir: regresa al menú anterior */
            printf("\nHuiste del combate.\n");
            terminar = 1;
        } else {
            printf("\nOpción no válida.\n");
        }
    }
}