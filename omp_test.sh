#!/bin/sh
#SBATCH --job-name=RSA_cracker_omp
#SBATCH --time=01:00:00
#SBATCH --output=results/omp_cracker.out
#SBATCH --nodes=8
#SBATCH --cpus-per-task=16

export OMP_NUM_THREADS=16

echo '===== OMP Cracker Test (64, 72, 80, 88 bits) ====='
./omp 64
./omp 72
./omp 80
./omp 88
