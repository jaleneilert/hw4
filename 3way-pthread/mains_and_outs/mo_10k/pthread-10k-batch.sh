#!/bin/bash -l

echo "I'm using $NUM_CORES cores"
//usr/bin/time -o /homes/jjeilert/proj4/hw4/3way-pthread/times/times_for_10k/time-${NUM_CORES}-$RANDOM.txt /homes/jjeilert/proj4/hw4/3way-pthread/mains_and_outs/mo_10k/pthreads-10k /homes/jjeilert/proj4/hw4/3way-pthread/times/times_for_10k/time-from-code-${NUM_CORES}-$RANDOM.txt  
