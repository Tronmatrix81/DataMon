# 1. Variables de configuración
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = datamon
SRCS = src/main.c src/game.c src/combat.c src/structures.c src/archives.c src/ui.c
OBJS = $(SRCS:.c=.o)

# 2. Regla principal (se ejecuta al escribir solo "make")
all: $(TARGET)

# 3. Enlazado final: une todos los .o en el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# 4. Regla patrón: compila cada .c individualmente a un archivo objeto .o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Limpieza de archivos generados
clean:
	rm -f $(OBJS) $(TARGET)