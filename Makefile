N?=10000000
P?=4
TYPE?=Naive
PROGRAM?=mpi_send_recv

main:
	rm -f $(PROGRAM)
	mpicc -o $(PROGRAM) ./src/$(TYPE)/$(PROGRAM).c -lm
	mpirun -n $P --oversubscribe ./$(PROGRAM) $N
	rm -f $(PROGRAM)
