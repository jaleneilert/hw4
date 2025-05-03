#!/bin/bash

for i in 1 2 4 8 16
do
    sbatch --job-name=my_job --output=slurm-%j.out --constraint=moles --time=0:10:00 --mem-per-cpu=1G --nodes=1 --ntasks-per-node=$i --export=ALL,NUM_CORES=$i --partition=killable.q ./mpi_10k_batch.sh
done