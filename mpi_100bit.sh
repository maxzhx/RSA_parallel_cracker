#!/bin/sh
#SBATCH --job-name=RSA_cracker_mpi
#SBATCH --time=12:00:00
#SBATCH --output=results/mpi_100.out
#SBATCH --nodes=8
#SBATCH --cpus-per-task=16

export OMP_NUM_THREADS=16

echo '===== MPI Cracker Test (100 bits) ====='
mpiexec -n 128 mpi 100
