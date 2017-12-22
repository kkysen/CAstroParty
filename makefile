
PROGRAM_NAME=game.out

all:
	gcc -o $(PROGRAM_NAME) main.c -lSDL2 -LSDL2main

clean:
	rm -f $(PROGRAM_NAME)
