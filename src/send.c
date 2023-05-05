#include <math.h>
#include <openmpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>

int min(int a, int b);

int get_number() {
  int number;

  printf("Por favor entre com o numero que voce quer analisar: \n");
  scanf("%d", &number);
  printf("\n");

  return number;
}

int main(int argc, char *argv[]) {
  int number = 550;

  MPI_Init(&argc, &argv);

  int number_of_processes, process_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  int amount_of_numbers_to_check = floor(sqrt(number)) - 1;

  int calculations_per_process =
      ceil((double)amount_of_numbers_to_check / number_of_processes);

  int start = process_rank * calculations_per_process + 2;
  int end =
      min(start + calculations_per_process - 1, amount_of_numbers_to_check + 1);

  printf("Process %d: Start %d, End %d\n", process_rank, start, end);

  MPI_Finalize();

  return 0;
}

int min(int a, int b) { return a < b ? a : b; }
