CC      = gcc
CFLAGS  = -Wall -Wextra -I../Inc
SRC     = main.c ordonnancement.c FIFO.c SJF.c SJRF.c RR.c Liste.c ProcStruct.c
TARGET  = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all clean re