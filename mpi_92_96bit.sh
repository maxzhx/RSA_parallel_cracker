#!/bin/sh
#SBATCH --job-name=RSA_cracker_mpi
#SBATCH --time=02:00:00
#SBATCH --output=results/mpi_92_96.out
#SBATCH --nodes=8
#SBATCH --cpus-per-task=16

export OMP_NUM_THREADS=16

echo '===== MPI Cracker Test (92, 96 bits) ====='
mpiexec -n 128 mpi 92
mpiexec -n 128 mpi 96
