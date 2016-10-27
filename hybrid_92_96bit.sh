#!/bin/sh
#SBATCH --job-name=RSA_cracker_hybrid
#SBATCH --time=02:00:00
#SBATCH --output=results/hybrid_92_96.out
#SBATCH --nodes=8
#SBATCH --cpus-per-task=16

export OMP_NUM_THREADS=16

echo '===== Hybrid Cracker Test (92, 96 bits) ====='
mpiexec -n 8 hybrid 92
mpiexec -n 8 hybrid 96
