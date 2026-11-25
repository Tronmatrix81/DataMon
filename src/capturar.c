mov movimientoshasheados[151];

// 1. Convierte el nombre del movimiento en un número del 0 al 150
int fhash(char cadena[])
{
    int hasheo = 0;
    for (int i = 0; cadena[i] != '\0'; i++)
    {
        hasheo = (hasheo * 31 + (unsigned char)cadena[i]) % 151;
    }
    return hasheo;
}

// 2. Lee movements.txt y guarda cada movimiento en su casilla hash
void leermovimientos(void)
{
    FILE *arch;
    char nombre_temp[50];
    int dano_temp;
    int pos;

    // Marcamos todas las casillas como vacías (-1)
    for (int i = 0; i < 151; i++)
    {
        movimientoshasheados[i].daño = -1;
    }

    arch = fopen("../data/movements.txt", "rt");
    if (arch != NULL)
    {
        for (int i = 0; i < 72; i++)
        {
            fscanf(arch, "%s %i", nombre_temp, &dano_temp);
            pos = fhash(nombre_temp);

            // Si la casilla ya está ocupada por otro ataque, avanza a la siguiente
            while (movimientoshasheados[pos].daño != -1)
            {
                pos = (pos + 1) % 151;
            }

            strcpy(movimientoshasheados[pos].movimiento, nombre_temp);
            movimientoshasheados[pos].daño = dano_temp;
        }
        fclose(arch);
    }
}

// 3. Función auxiliar muy corta para buscar el daño de UN solo movimiento
int buscar_dano(char nombre_mov[])
{
    int pos = fhash(nombre_mov);

    while (movimientoshasheados[pos].daño != -1)
    {
        if (strcmp(movimientoshasheados[pos].movimiento, nombre_mov) == 0)
        {
            return movimientoshasheados[pos].daño;
        }
        pos = (pos + 1) % 151;
    }
    return 1; // Si por algo no lo encuentra, devuelve 1
}

// 4. Le pone el daño real (daño base * nivel) a los 4 ataques de cualquier Pokemon
void asociardañomovimientos(Pokemon *poke)
{
    for (int i = 0; i < 4; i++)
    {
        int dano_base = buscar_dano(poke->movements[i].movimiento);
        poke->movements[i].daño = dano_base * poke->lvl;
    }
}