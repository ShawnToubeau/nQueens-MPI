all: program1

program1: main.c
	mpicc -fopenmp -o main.x main.c assistFunctions.c

clean:
	rm *.x
