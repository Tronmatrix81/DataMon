CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = datamon
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ejecutar siempre desde la raiz para que encuentre data/ y saves/
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
