include/ (Los planos): Guarda únicamente archivos .h. Es la "tabla de contenidos" de el programa para que todos los archivos sepan qué funciones existen.

src/ (El motor): Guarda únicamente archivos .c. Aquí vive toda la lógica real del programa (los if, ciclos, punteros y operaciones).

data/ (Datos fijos del juego): Guarda archivos que el juego necesita para arrancar (como el catálogo base de especies de DataMon) o archivos globales (el Ranking).

saves/ (Partidas guardadas): Empieza vacía. Cada vez que un jugador guarda su partida con un nombre personalizado, el archivo se crea aquí adentro para no mezclar partidas con el código fuente.

Un Makefile es un archivo sin extension que guarda el comando inicial para compilar y ejecutar todos los archivos .c y .h para dartelos listos en un solo programa
