CC = gcc -ggdb -std=gnu99 -Wall -Werror -Wextra -O3 -march=native

SRC_DIR = src
BIN_DIR = bin

PROGRAM_NAME = game.out

CFLAGS = -lSDL2 -lSDL2main

rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

HEADERS = $(wildcard $(call rwildcard, $(SRC_DIR), *.h))

SRCS = $(wildcard $(call rwildcard, $(SRC_DIR), *.c))

OBJS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))


all: bin_dir_exists $(PROGRAM_NAME)

# Use all objects to generate executable
$(PROGRAM_NAME): $(HEADERS) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Compile all objects
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

bin_dir_exists:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/util

run: all
	./$(PROGRAM_NAME)

clean:
	rm -f $(PROGRAM_NAME)
	rm -rf $(BIN_DIR)
