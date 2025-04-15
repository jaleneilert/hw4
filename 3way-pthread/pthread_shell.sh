#!/bin/bash
#SBATCH --job-name=my_job          # Name of the job
#SBATCH --output=slurm-%j.out      # Output file (%j = job ID)
#SBATCH --time=01:10:00            # Max run time (hh:mm:ss)
#SBATCH --mem-per-cpu=1G           # Memory per core
#SBATCH --cpus-per-task=1         # Number of CPU cores
#SBATCH --nodes=1                  # Run on 1 node
#SBATCH --ntasks=1                 # 1 task total (read the wikidump)
#SBATCH --constraint=dwarves       # use dwarves not dragons

# Load any modules if needed, e.g.
# module load CMake/3.23.1-GCCcore-11.3.0 foss/2022a OpenMPI/4.1.4-GCC-11.3.0 CUDA/11.7.0

# Run program
./build/HW4_test
