
CC=gcc

SRC_DIR=src
BIN_DIR=bin

PROGRAM_NAME=game.out

CFLAGS=-lSDL2 -lSDL2main

SRCS=$(wildcard $(SRC_DIR)/*.c)

OBJS=$(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))


all: bindirexists $(PROGRAM_NAME)

# Use all objects to generate executable
$(PROGRAM_NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Compile all objects
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

bindirexists:
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(PROGRAM_NAME)
	rm -rf $(BIN_DIR)
# Old stuff
#all:
#	gcc -o $(PROGRAM_NAME) main.c -lSDL2 -LSDL2main
#
#clean:
#	rm -f $(PROGRAM_NAME)
