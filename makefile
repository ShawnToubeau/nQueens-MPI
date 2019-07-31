all: program1 assistFunctions

program1: main.c
	mpicc -fopenmp -o main.x main.c

assistFunctions: 
	mpicc -fopenmp -o assistFunctions.x assistFunctions.c

clean:
	rm *.x
