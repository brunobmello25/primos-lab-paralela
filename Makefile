N?=10000000
P?=4
TYPE?=mpi_send_recv

main:
	rm -f $(TYPE)
	mpicc -o $(TYPE) ./src/$(TYPE).c -lm
	mpirun -n $P --oversubscribe ./$(TYPE) $N
	rm -f $(TYPE)
