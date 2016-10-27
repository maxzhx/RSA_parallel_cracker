#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include "omp.h"
#include <sys/time.h>

int threads;
void primeFactor(mpz_t result, mpz_t n) {
  mpz_t b;
  mpz_t sq;
  mpz_init(b);
  mpz_init(sq);

  mpz_mod_ui (b, n, 2);
  if (mpz_cmpabs_ui(b, 0) == 0) {
    mpz_set_ui(result, 2);
    return;
  }

  mpz_add(sq, sq, n);
  mpz_sqrt(sq, sq);
  mpz_mod_ui(b, sq, 2);
  mpz_sub_ui(sq, sq, !mpz_cmpabs_ui(b, 0));
  mpz_clear(b);

  int flag = 0;
  #pragma omp parallel
  {
    threads = omp_get_num_threads();
    #pragma omp master
    printf("... cracking with %d threads ...\n", threads);
    mpz_t i;
    mpz_t t;
    mpz_init(i);
    mpz_init(t);
    int rank = omp_get_thread_num();
    mpz_sub_ui(i, sq, rank*2);
    // gmp_printf("thread %d, %Zd\n", threads, i);

    // n must be odd
    for (; mpz_cmp_ui(i, 3) >= 0 && !flag; mpz_sub_ui(i, i, 2*threads)) {
      mpz_mod(t, n, i);
      if ( mpz_cmp_ui(t, 0) == 0) {
        printf("... found by thread %d ...\n", rank);
        flag = 1;
        mpz_set(result, i);
        break;
      }
    }
    mpz_clear(i);
    mpz_clear(t);
  }
  mpz_clear(sq);
}

int main(int argc, char *argv[]) {
  struct timeval start_t, end_t;
  char modulu[256];
  char pe[256];
  mpz_t a, e, p, q, fin, d;
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
    default :
    option = 12;
    mpz_set_str(a, "3233", 10);
  }

  mpz_set_str(e, "65537", 10);

  printf("=== Public Key ===\n");
  printf("{\n  bit: %d\n", option);
  gmp_printf ("  modulus: %Zd\n", a);
  gmp_printf ("  publicExponent: %Zd\n}\n\n",  e);

  gettimeofday(&start_t, NULL);
  primeFactor(p, a);
  gettimeofday(&end_t, NULL);
  printf("exe_time: %f\n\n", ((end_t.tv_sec  - start_t.tv_sec) * 1000000u + end_t.tv_usec - start_t.tv_usec) / 1.e6);

  printf("=== Private Key ===\n{\n");
  // cal -d
  mpz_tdiv_q (q, a, p);
  gmp_printf("  prime1:%Zd\n  prime2:%Zd\n", p, q);
  mpz_sub_ui(p, p, 1UL);
  mpz_sub_ui(q, q, 1UL);
  mpz_mul(fin, p, q);
  mpz_invert(d, e, fin);
  gmp_printf("  privateExponent:%Zd\n}\n", d);

  mpz_clear(a);
  mpz_clear(e);
  mpz_clear(p);
  mpz_clear(q);
  mpz_clear(fin);
  mpz_clear(d);
  return 0;
}
