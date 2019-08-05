all: program1 program2

test:
	gcc -o test.o test.c mutate.c conflict.c assistFunctions.c

program1: main.c
	mpicc -fopenmp -o main.x main.c assistFunctions.c conflict.c

parallel: parallel.c
	mpicc -o parallel.x parallel.c assistFunctions.c conflict.c mutate.c

serial: serial.c
	gcc -o serial.x serial.c assistFunctions.c conflict.c mutate.c

clean:
	rm *.x
