all: serial build-parallel
build-parallel:
	mpicc amigos_paralel.c -o amigos
run-parallel: build-parallel
	mpirun -np 4 ./amigos 
serial:
	gcc -o amigos amigos.c