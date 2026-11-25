#include <stdio.h>
#include <stdlib.h>

typedef struct pokemon{

    char nombre[50];
    char t1[50]; //tipo de elemento 1
    char t2[50]; //tipo de elemento 2
    int vida;
    struct pokemon *sig;
} poke;

poke *arreglo[2];

int partidahecha=0;
int main(void){

    
    int opc;
    //bueno puedo hacer que de tu eqipo agarre uno random y se mueva de coso a coso o sea que sean dos listas,  una de equipo, bueno dos arreglos no? o no a ver si podria ser arreglo 1 y 2 de que el primer arreglo sea una lista y se pase
    //de inventario a equipo, ok......
    while(1){

        printf("\n1)Ver equipo (mostar todos los datamon que ya ha capturado: nombre, nivel, vida)\n2)Buscar DataMon salvaje\n3)Ir a Arena\n4)Ver Mochila (los objetos se ganan en las peleas en la arena)\n5)Centro de curación, recuperar la vida (solo si tiene pociones de vida en la mochila, debe indicar a cual datamon)\n6)Guardar partida (guardar el equipo y la mochila en un archivo de nombres dados por el usuario)\n7)Volver al menú principal\n");
        scanf(opc);
        if (partidahecha==0)
        {
            partidahecha=1;


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
