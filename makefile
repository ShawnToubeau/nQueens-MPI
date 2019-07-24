all: program1

program1: main.c
	gcc -o main.x main.c

clean:
	rm *.x