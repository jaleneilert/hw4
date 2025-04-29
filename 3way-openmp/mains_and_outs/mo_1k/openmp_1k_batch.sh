#!/bin/bash -l

echo "I'm using $NUM_CORES cores"
//usr/bin/time -o /homes/jjeilert/proj4/hw4/3way-openmp/times/times_for_1k/time-${NUM_CORES}-$RANDOM.txt /homes/jjeilert/proj4/hw4/3way-openmp/mains_and_outs/mo_1k/openmp-1k /homes/jjeilert/proj4/hw4/3way-openmp/times/times_for_1k/time-from-code-${NUM_CORES}-$RANDOM.txt