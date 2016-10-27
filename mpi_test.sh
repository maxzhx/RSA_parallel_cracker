#!/bin/sh
#SBATCH --job-name=RSA_cracker_mpi
#SBATCH --time=01:00:00
#SBATCH --output=results/mpi_cracker.out
#SBATCH --nodes=8
#SBATCH --cpus-per-task=16

export OMP_NUM_THREADS=16

echo '===== MPI Cracker Test (64, 72, 80, 88 bits) ====='
mpiexec -n 128 mpi 64
mpiexec -n 128 mpi 72
mpiexec -n 128 mpi 80
mpiexec -n 128 mpi 88
