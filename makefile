all: program1 assistFunctions

program1: main.c
	gcc -o main.x main.c

assistFunctions: 
	gcc -o assistFunctions.x assistFunctions.c -fopenmp

clean:
	rm *.x
