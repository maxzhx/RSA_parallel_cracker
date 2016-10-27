#!/bin/sh
#SBATCH --job-name=RSA_cracker_hybrid
#SBATCH --time=01:00:00
#SBATCH --output=results/hybrid_cracker.out
#SBATCH --nodes=8
#SBATCH --cpus-per-task=16

export OMP_NUM_THREADS=16

echo '===== Hybrid Cracker Test (64, 72, 80, 88 bits) ====='
mpiexec -n 8 hybrid 64
mpiexec -n 8 hybrid 72
mpiexec -n 8 hybrid 80
mpiexec -n 8 hybrid 88
