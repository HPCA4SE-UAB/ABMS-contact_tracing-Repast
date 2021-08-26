#!/bin/bash -l
# The -l above is required to get the full environment with modules

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A prace-covid19-60

# The name of the script is myjob
#SBATCH -J Repast

# Only 1 hour wall-clock time will be given to this job
#SBATCH -t 00:20:00

# Number of nodes
#SBATCH --nodes=1
# Number of MPI processes per node
#SBATCH --ntasks-per-node=32

#If you would like to execute your job exclusively on haswell(2 x Xeon E5-2698v3 Haswell 2.3 GHz CPUs (16 cores per CPU) nodes you can use…
#SBATCH -C Haswell

#SBATCH --output=output/outputslurm.%N.%j.out
#SBATCH --error=output/outputslurm.%N.%j.err
 
# Load modules
module swap PrgEnv-cray PrgEnv-gnu
module load boost/1.71.0-gcc-8.3.0
module load alps


# Program to execute
export TAU_MAKEFILE=/cfs/nobackup/c/cesaar/sfw/tau-2.29.1/craycnl/lib/Makefile.tau-gnu-mpi-pdt
export TAU_OPTIONS=-optCompInst
export PATH=/cfs/nobackup/c/cesaar/sfw/tau-2.29.1/craycnl/bin/:$PATH
export LD_LIBRARY_PATH=/cfs/nobackup/c/cesaar/sfw/lib:/cfs/klemming/nobackup/c/cesaar/sfw/tau-2.29.1/craycnl/lib/:/pdc/vol/boost/1.71.0-gcc-8.3.0/lib:$LD_LIBRARY_PATH
#echo $LD_LIBRARY_PATH
#export TAU_COMM_MATRIX=1
#export TAU_TRACE=1


srun -n 32 bin/Model.exe props/config.props props/model.props >output/sortida.txt;
#pprof -p > output/pprof_out;

