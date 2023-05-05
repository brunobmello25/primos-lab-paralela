N?=4

send:
	rm -f send
	mpicc -o send ./src/send.c -lm
	mpirun -n $N --oversubscribe ./send
	rm -f send
