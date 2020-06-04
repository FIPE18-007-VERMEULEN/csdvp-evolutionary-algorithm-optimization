#!/bin/bash
#SBATCH -o main.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH --mem=4G
#SBATCH -p defq

/home/jeremie.humeau/private/gitProject/csdvp-evolutionary-algorithm-optimization/build/application/ceao -G=10000 -P=100 -S=$SLURM_ARRAY_TASK_ID -C=$C -c=$c -A=$A --outputfile=$O -n=$A -B=$B -x=$x -X=$X -F=$F


exit 0
