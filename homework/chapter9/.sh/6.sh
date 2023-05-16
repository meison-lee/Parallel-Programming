#!/bin/sh
#PBS -P ACD110146
#PBS -N r
#PBS -q ctest
#PBS -l select=1:ncpus=6:mpiprocs=6
#PBS -l place=scatter
#PBS -l walltime=00:01:00
#PBS -j n
module purge
module load intel/2018_u1
cd $PBS_O_WORKDIR
echo $PBS_O_WORKDIR

date
mpirun ./R 