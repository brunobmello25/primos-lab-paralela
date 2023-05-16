#include <math.h>
#include <openmpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>

int primo(long int n) {
  int i;
  for (i = 3; i < (int)(sqrt(n) + 1); i += 2) {
    if (n % i == 0)
      return 0;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  double t_inicial, t_final;
  int cont = 0, total = 0;
  long int i, n;
  int meu_ranque, num_procs, inicio, salto;
  MPI_Request request;

  if (argc < 2) {
    printf("Valor inválido! Entre com um valor do maior inteiro\n");
    return 0;
  } else {
    n = strtol(argv[1], (char **)NULL, 10);
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  t_inicial = MPI_Wtime();

  inicio = 3 + meu_ranque * 2;
  salto = num_procs * 2;
  for (i = inicio; i <= n; i += salto) {
    if (primo(i) == 1)
      cont++;
  }

  // Caso o número de processos seja maior que 1, então será
  // necessário que os processos enviem os resultados para o 0.
  if (num_procs > 1) {
    // Caso o ranque seja diferente de 0, então é preciso enviar o resultado
    // para o processo 0.
    if (meu_ranque != 0) {
      MPI_Isend(&cont, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
      // Caso o ranque seja 0, então é preciso apenas receber os resultados e
      // somar na variável total.
    } else {
      total = cont;
      // Laço para executar o MPI_Recv para a quantidade de processos que irão
      // enviar para o processo 0.
      for (int source = 1; source < num_procs; source++) {
        // Endereço onde os dados serão armazenados quando chegarem ao processo
        // 0.
        int received_cont;
        MPI_Recv(&received_cont, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        total += received_cont;
      }
    }
    // Caso só exista um processo, então basta esse calcular a quantidade de
    // primos e colocar na variável total.
  } else {
    total = cont;
  }
  t_final = MPI_Wtime();

  if (meu_ranque == 0) {
    total += 1; /* Acrescenta o dois, que também é primo */
    printf("Quant. de primos entre 1 e n: %d \n", total);
    printf("Tempo de execucao: %1.6f \n", t_final - t_inicial);
  }
  MPI_Finalize();
  return (0);
}
