#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
-Nueva Partida
-Cargar Partida
        -Ver equipo (mostar todos los datamon que ya ha capturado: nombre, nivel, vida)
        Buscar DataMon salvaje
            -Luchar
            Capturar (Solo se puede capturar si lo debilita después de luchar), al capturar agregar a una estructura (arreglo, lista, tabla hash, arbol)
            Huir (volver al menu anterior)


        Ir a Arena
            Ataque rapido
            Chispa, si se elige, indicar: Bytechu usó Chispa - Rockbit perdió 10 HP (Ustedes determinan la fuerza del ataque, aleatorio o por nivel)
            Cambiar criatura, elegir otra criatura de tu equipo
            Usar poción (de la mochila para incrementar vida)

        Ver Mochila (los objetos se ganan en las peleas en la arena)
        Centro de curación, recuperar la vida (solo si tiene pociones de vida en la mochila, debe indicar a cual datamon)
        Guardar partida (guardar el equipo y la mochila en un archivo de nombres dados por el usuario)
        Volver al menú principal

-Ranking
-Salir



*/

typedef struct pokemon
{
    char nombre[50];
    char t1[50]; // tipo de elemento 1
    char t2[50]; // tipo de elemento 2
    int nivel;
    int vida;
    struct pokemon *sig;
} poke;

poke *arreglo[2];

int partidahecha = 0;
int main(int argc, const char *argv[])
{
    int opc = -1, numero;
    poke *mipoke;
    srand(time(NULL));
    char linea[100];
    FILE *arch = fopen("datamones.txt", "rt");

    while (1)
    {
        printf("\n1)Ver equipo (mostar todos los datamon que ya ha capturado: nombre, nivel, vida)\n2)Buscar DataMon salvaje\n3)Ir a Arena\n4)Ver Mochila (los objetos se ganan en las peleas en la arena)\n5)Centro de curación, recuperar la vida (solo si tiene pociones de vida en la mochila, debe indicar a cual datamon)\n6)Guardar partida (guardar el equipo y la mochila en un archivo de nombres dados por el usuario)\n7)Volver al menú principal\n");
        scanf("%i", &opc);
        if (partidahecha == 0)
        {
            partidahecha = 1;

            numero = rand() % 1037;

            if (arch != NULL)
            {
                if (fgets(linea, sizeof(linea), arch) != NULL)
                {
                    // sscanf devuelve 3 si encontró: nombre, t1 y t2
                    // o devuelve 2 si solo encontró: nombre y t1
                    int leidos = sscanf(linea, "%s %s %s", mipoke->nombre, mipoke->t1, mipoke->t2);

                    if (leidos == 2)
                    {
                        // Solo tiene un tipo; dejamos t2 vacío (o puedes poner "Ninguno")
                        strcpy(mipoke->t2, "");
                    }
                }
            }
        }

        switch (opc)
        {
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
        case 6:
            /* code */
            break;
        case 7:
            /* code */
            break;

        default:
            break;
        }
    }

    return 0;
}
