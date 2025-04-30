#!/bin/bash -l

echo "I'm using $NUM_CORES cores"
//usr/bin/time -o /homes/jjeilert/proj4/hw4/3way-openmp/times/times_for_10k/time-${NUM_CORES}-$RANDOM.txt /homes/jjeilert/proj4/hw4/3way-openmp/mains_and_outs/mo_10k/openmp-10k /homes/jjeilert/proj4/hw4/3way-openmp/times/times_for_10k/time-from-code-${NUM_CORES}-$RANDOM.txt