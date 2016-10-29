#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include "mpi.h"
#include <sys/time.h>

#define MASTER 0
#define WORKTAG 1
#define DIETAG 2

int my_rank, num_procs;
long t_size = 0;

void mpi_primeFactors(mpz_t n) {
  int flag = 0;
  int count = 0;
  MPI_Request request;
  MPI_Status status;
  mpz_t i;
  mpz_t b;
  mpz_t sq;
  mpz_init(i);
  mpz_init(b);
  mpz_init(sq);
  mpz_add(sq, sq, n);
  mpz_sqrt(sq, sq);
  mpz_mod_ui(b, sq, 2);
  mpz_sub_ui(sq, sq, !mpz_cmp_ui(b, 0));

  MPI_Irecv(NULL, 0, MPI_INT, MASTER, DIETAG, MPI_COMM_WORLD, &request);
  for (mpz_sub_ui(i, sq, (my_rank-1)*2); mpz_cmp_ui(i, 3) >= 0; mpz_sub_ui(i, i, 2*(num_procs-1))) {
    // check if other process succeed, every 1000000 time, to increase efficiency
    if (count++ % 1000000 == 0) {
      MPI_Test(&request, &flag, &status);
      if (flag) {
        break;
      }
    }
    mpz_mod(b, n, i);
    if (mpz_cmp_ui(b, 0) == 0) {
      gmp_printf("... found by node %d ...\n", my_rank);
      long size = sizeof(char);
      long numb = 8*size - 0;
      long count = (mpz_sizeinbase (i, 2) + numb-1) / numb;
      t_size = count * size;
      MPI_Send(&t_size, 1, MPI_LONG, MASTER, WORKTAG, MPI_COMM_WORLD);
      char *p = (char *)malloc (t_size);
      mpz_export(p, NULL, 1, 1, 0, 0, i);
      // printf("p====%s\n", p);
      MPI_Send(p, t_size, MPI_CHAR, MASTER, WORKTAG, MPI_COMM_WORLD);
      break;
    }
  }
  mpz_clear(b);
  mpz_clear(i);
  mpz_clear(sq);
  MPI_Send(0,0,MPI_LONG,MASTER,DIETAG,MPI_COMM_WORLD);
  return ;
}

int main(int argc, char *argv[]) {
  MPI_Status status;
  char *ex;
  /* Initialize the infrastructure necessary for communication */
  MPI_Init(&argc, &argv);
  /* Identify this process */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  /* Find out how many total processes are active */
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  mpz_t a, e;
  mpz_t p, q;
  mpz_t fin, d;
  char modulu[256];
  char pe[256];
  mpz_init(a);
  mpz_init(e);
  mpz_init(p);
  mpz_init(q);
  mpz_init(fin);
  mpz_init(d);

  int option = atoi(argv[1]);
  switch (option) {
    case 64:
      mpz_set_str(a, "e7549c6eccd9c65b", 16);
      break;
    case 68:
      mpz_set_str(a, "0b61ae7e696382a695", 16);
      break;
    case 72:
      mpz_set_str(a, "00bd18e1531b4f7be665", 16);
      break;
    case 80:
      mpz_set_str(a, "00e707e15c8a39b4ac3f15", 16);
      break;
    case 84:
      mpz_set_str(a, "0d908c386e24d61c971e6f", 16);
      break;
    case 88:
      mpz_set_str(a, "00dd09795eca85a11872c533", 16);
      break;
    case 92:
      mpz_set_str(a, "0ded51d6e5593dacd1c0c505", 16);
      break;
    case 96:
      mpz_set_str(a, "00dd1733bdb2c0be2f1f47b8af", 16);
      break;
    case 100:
      mpz_set_str(a, "0c9d79bf2dfabb76079d8defc5", 16);
      break;
    case 104:
      mpz_set_str(a, "00d8614305db62ad1b9db8ded2cf", 16);
      break;
    case 108:
      mpz_set_str(a, "095d4fe30ca5f84aa42a9daab0fb", 16);
      break;
    case 112:
      mpz_set_str(a, "00d9a7f5e675dc36b3066ca8d81227", 16);
      break;
    default :
      option = 12;
      mpz_set_str(a, "3233", 10);
  }

  mpz_set_str(e, "65537", 10);

  if (my_rank == 0) {
    struct timeval start_t, end_t;
    gettimeofday(&start_t, NULL);
    printf("=== Public Key ===\n");
    printf("{\n  bit: %d\n", option);
    gmp_printf ("  modulus: %Zd\n", a);
    gmp_printf ("  publicExponent: %Zd\n}\n\n",  e);
    printf("... cracking with %d nodes ...\n", num_procs);
    int finished = 0;
    do {
      MPI_Recv(&t_size,1,MPI_LONG,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
      if (status.MPI_TAG == DIETAG) {
        finished++;
      } else {
        // printf("received%ld\n", t_size);
        ex = (char *)malloc(t_size);
        MPI_Recv(ex, t_size,MPI_CHAR,MPI_ANY_SOURCE,WORKTAG,MPI_COMM_WORLD,&status);
        gettimeofday(&end_t, NULL);
        printf("exe_time: %f\n", ((end_t.tv_sec  - start_t.tv_sec) * 1000000u + end_t.tv_usec - start_t.tv_usec) / 1.e6);
        // printf("ex====%s\n", ex);
        mpz_import(p, t_size , 1, sizeof(*ex), 0, 0, ex);
        // gmp_printf("p!! %Zd\n", p);
        printf("... waiting for exit of other processes ...\n\n");
        int i;
        for (i = 1; i < num_procs; i++) {
          MPI_Send(NULL, 0, MPI_INT, i, DIETAG, MPI_COMM_WORLD);
        }
        break;
      }
    } while (finished < num_procs-1);

    printf("=== Private Key ===\n{\n");
    // cal -d
    mpz_tdiv_q (q, a, p);
    gmp_printf("  prime1:%Zd\n  prime2:%Zd\n", p, q);
    mpz_sub_ui(p, p, 1UL);
    mpz_sub_ui(q, q, 1UL);
    mpz_mul(fin, p, q);
    mpz_invert(d, e, fin);
    gmp_printf("  privateExponent:%Zd\n}\n", d);
  } else {
    mpi_primeFactors(a);
  }

  mpz_clear(a);
  mpz_clear(e);
  mpz_clear(p);
  mpz_clear(q);
  mpz_clear(fin);
  mpz_clear(d);

  MPI_Finalize();
  return 0;
}
