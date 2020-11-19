#!/bin/bash
#SBATCH -o main.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH --mem=4G
#SBATCH -p defq

/home/jeremie.humeau/private/gitProject/csdvp-evolutionary-algorithm-optimization/build/application/ceao -G=$G -P=10 -S=$SLURM_ARRAY_TASK_ID -C=$C -c=$c -R=$R -Q=$Q -A=$A --outputfile=$O -n=$n -N=$N -r=$r -q=$q -B=$B -x=$x -X=$X -F=$F -T=10


exit 0
