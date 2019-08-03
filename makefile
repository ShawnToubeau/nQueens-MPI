all: program1

test:
	gcc -o test.o test.c mutate.c conflict.c assistFunctions.c

program1: main.c
	mpicc -fopenmp -o main.x main.c mutate.c assistFunctions.c conflict.c

clean:
	rm *.x
