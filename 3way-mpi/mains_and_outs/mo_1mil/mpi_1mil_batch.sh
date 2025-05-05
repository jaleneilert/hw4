#!/bin/bash -l

echo "I'm using $NUM_CORES cores"
//usr/bin/time -o /homes/jjeilert/proj4/hw4/3way-mpi/times/times_for_1mil/time-${NUM_CORES}-$RANDOM.txt /homes/jjeilert/proj4/hw4/3way-mpi/mains_and_outs/mo_1mil/mpi-1mil /homes/jjeilert/proj4/hw4/3way-mpi/times/times_for_1mil/time-from-code-${NUM_CORES}-$RANDOM.txt