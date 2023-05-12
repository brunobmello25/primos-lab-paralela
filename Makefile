N?=10000000
P?=4

send:
	rm -f send
	mpicc -o send ./src/send.c -lm
	mpirun -n $P --oversubscribe ./send $N
	rm -f send
