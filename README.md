# COMP90025 Project2 RSA_cracker

## Library:
Install GMP library first

## compile linear:
```sh
gcc -lgmp linear_cracker.c
```

## compile OMP:
```sh
gcc -fopenmp -lgmp omp_cracker.c
```

## compile MPI:
```sh
mpicc -lgmp mpi_cracker.c
```

## compile OMP & MPI hybrid:
```sh
mpicc -lgmp -openmp hybrid_cracker.c
```

## to run give it a bit number as input:
### now support (64, 68, 72, 80, 84, 88, 92, 96)
```sh
./a.out 64
or
mpiexec -n 4 ./a.out 72
```
