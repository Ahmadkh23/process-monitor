CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = process_monitor
OBJ = main.o monitor.o utils.o logger.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c monitor.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)