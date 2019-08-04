all: program1 program2

test:
	gcc -o test.o test.c mutate.c conflict.c assistFunctions.c

program1: main.c
	mpicc -fopenmp -o main.x main.c assistFunctions.c conflict.c

program2: parallel.c
	mpicc -fopenmp -o parallel.x parallel.c assistFunctions.c conflict.c mutate.c


clean:
	rm *.x
