#!/bin/bash -l

echo "I'm using $NUM_CORES cores"
//usr/bin/time -o /homes/jjeilert/proj4/hw4/3way-openmp/times/times_for_100k/time-${NUM_CORES}-$RANDOM.txt /homes/jjeilert/proj4/hw4/3way-openmp/mains_and_outs/mo_100k/openmp-100k /homes/jjeilert/proj4/hw4/3way-openmp/times/times_for_100k/time-from-code-${NUM_CORES}-$RANDOM.txt