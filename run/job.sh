#!/bin/sh
#SBATCH -o main.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH --mem=10G
#SBATCH -p defq

/home/jeremie.humeau/private/gitProject/csdvp-evolutionary-algorithm-optimization/build/application/ceao -B=50 -S=$S -C=$C -c=$c -A=$A --outputfile=$O -n=$A

exit 0
