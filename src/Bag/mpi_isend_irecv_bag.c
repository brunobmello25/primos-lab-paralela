#include <math.h>
#include <openmpi-x86_64/mpi.h>
#include <openmpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define TAMANHO 500000

int primo(int n) {
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
  int i, n;
  int meu_ranque, num_procs, inicio, dest, raiz = 0, tag = 1, stop = 0;
  MPI_Status estado;
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

  /* Se houver menos que dois processos aborta */
  if (num_procs < 2) {
    printf("Este programa deve ser executado com no mínimo dois processos.\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
    return (1);
  }

  /* Registra o tempo inicial de execução do programa */
  t_inicial = MPI_Wtime();

  /* Envia pedaços com TAMANHO números para cada processo */
  if (meu_ranque == 0) {
    for (dest = 1, inicio = 3; dest < num_procs && inicio < n;
         dest++, inicio += TAMANHO) {
      MPI_Isend(&inicio, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &request);
    }

    /* Fica recebendo as contagens parciais de cada processo */
    while (stop < (num_procs - 1)) {
      MPI_Irecv(&cont, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                &request);
      MPI_Wait(&request, &estado);
      total += cont;
      dest = estado.MPI_SOURCE;
      if (inicio > n) {
        tag = 99;
        stop++;
      }
      /* Envia um nvo pedaço com TAMANHO números para o mesmo processo*/
      MPI_Isend(&inicio, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &request);
      inicio += TAMANHO;
    }
  } else {
    /* Cada processo escravo recebe o início do espaço de busca */
    while (estado.MPI_TAG != 99) {
      MPI_Irecv(&inicio, 1, MPI_INT, raiz, MPI_ANY_TAG, MPI_COMM_WORLD,
                &request);
      MPI_Wait(&request, &estado);

      if (estado.MPI_TAG != 99) {
        for (i = inicio, cont = 0; i < (inicio + TAMANHO) && i < n; i += 2)
          if (primo(i) == 1)
            cont++;
        /* Envia a contagem parcial para o processo mestre */
        MPI_Isend(&cont, 1, MPI_INT, raiz, tag, MPI_COMM_WORLD, &request);
      }
    }
    /* Registra o tempo final de execução */
    t_final = MPI_Wtime();
  }

  if (meu_ranque == 0) {
    t_final = MPI_Wtime();
    total += 1; /* Acrescenta o 2, que é primo */
    printf("Quant. de primos entre 1 e %d: %d \n", n, total);
    printf("Tempo de execucao: %1.6f \n", t_final - t_inicial);
  }
  /* Finaliza o programa */
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return (0);
}

void print_if_root(int rank, char *data) {
  if (rank == 0) {
    printf("%s\n", data);
  }
}
