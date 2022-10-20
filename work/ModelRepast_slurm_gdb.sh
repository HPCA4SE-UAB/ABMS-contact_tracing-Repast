#!/bin/bash -l
#SBATCH --partition=global
#SBATCH --nodes=1
#SBATCH --ntasks=64                  # Number of MPI ranks
#SBATCH --exclusive
#SBATCH --nodelist=stanlee
#SBATCH --time=10:00:00
#SBATCH --output=output/outputslurm.%N.%j.out
#SBATCH --error=output/outputslurm.%N.%j.err
#SBATCH --mail-type=END
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=amoreno@euss.cat

# Enable modules
#source /opt/Modules/3.2.9/init/Modules4bash.sh

# Load modules
# module load gcc/4.9.1
# module load mpich/3.2
module load openmpi/3.0.4


# Program to execute
#export TAU_MAKEFILE=/lex/shared/projects/agent-base-modeling/sfw/tau-2.26.3/x86_64/lib/Makefile.tau-mpi-pdt
#export TAU_OPTIONS=-optCompInst
#export PATH=/lex/shared/projects/agent-base-modeling/sfw/tau-2.26.3/x86_64/bin/:$PATH
#export TAU_COMM_MATRIX=1
export LD_LIBRARY_PATH=/lex/shared/projects/agent-base-modeling/sfw/repast_hpc-2.3.1/lib/:/lex/shared/projects/agent-base-modeling/sfw/boost-1_61_0/lib/:$LD_LIBRARY_PATH
#export TAU_TRACE=1

#srun -n 4 bin/Model.exe props/config.props props/model.props >output/sortida.txt;

ulimit -c unlimited
mpirun -n 64 gdb -ex 'run' -ex 'generate-core-file' --args bin/Model.exe props/config.props props/model.props >output/sortida.txt;
#mpirun -n 64 bin/Model.exe props/config.props props/model.props >output/sortida.txt;

#pprof -p > output/pprof_out;
