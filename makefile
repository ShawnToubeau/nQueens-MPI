all: serial parallel

test:
	gcc -o test.o test.c mutate.c conflict.c assistFunctions.c

parallel: parallel.c
	mpicc -o parallel.x parallel.c assistFunctions.c conflict.c mutate.c

serial: serial.c
	gcc -o serial.x serial.c assistFunctions.c conflict.c mutate.c

clean:
	rm *.x
