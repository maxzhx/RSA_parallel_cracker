#!/bin/sh
#SBATCH --job-name=RSA_cracker_linear
#SBATCH --time=01:00:00
#SBATCH --output=results/linear_cracker.out
#SBATCH --nodes=8
#SBATCH --cpus-per-task=16

export OMP_NUM_THREADS=16

echo '===== Linear Cracker Test (64, 72, 80, 88 bits) ====='
./linear 64
./linear 72
./linear 80
./linear 88
