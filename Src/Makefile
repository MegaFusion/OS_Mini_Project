CC      = gcc
CFLAGS  = -Wall -Wextra -I../Inc
SRC     = main.c ordonnancement.c SJF.c SJRF.c Liste.c ProcStruct.c calcul_temps.c lecture_fichier.c
TARGET  = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all clean re