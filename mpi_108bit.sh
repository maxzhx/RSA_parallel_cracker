#!/bin/sh
#SBATCH --job-name=RSA_cracker_mpi
#SBATCH --time=24:00:00
#SBATCH --output=results/mpi_108.out
#SBATCH --nodes=8
#SBATCH --cpus-per-task=16

export OMP_NUM_THREADS=16

echo '===== MPI Cracker Test (108 bits) ====='
mpiexec -n 128 mpi 108
