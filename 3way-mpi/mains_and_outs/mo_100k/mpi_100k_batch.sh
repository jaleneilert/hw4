#!/bin/bash -l

echo "I'm using $NUM_CORES cores"
//usr/bin/time -o /homes/jjeilert/proj4/hw4/3way-mpi/times/times_for_100k/time-${NUM_CORES}-$RANDOM.txt /homes/jjeilert/proj4/hw4/3way-mpi/mains_and_outs/mo_100k/mpi-100k /homes/jjeilert/proj4/hw4/3way-mpi/times/times_for_100k/time-from-code-${NUM_CORES}-$RANDOM.txt