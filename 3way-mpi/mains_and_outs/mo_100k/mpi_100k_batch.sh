#!/bin/bash -l

echo "I'm using $NUM_CORES cores"
//usr/bin/time -o /homes/jakeesmith/hw4/3way-mpi/times/times_for_1k/time-${NUM_CORES}-$RANDOM.txt /homes/jakeesmith/hw4/3way-mpi/mains_and_outs/mo_100k/mpi_100k /homes/jakeesmith/hw4/3way-mpi/times/times_for_100k/time-from-code-${NUM_CORES}-$RANDOM.txt