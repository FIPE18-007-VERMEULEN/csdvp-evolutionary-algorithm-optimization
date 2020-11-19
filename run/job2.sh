#!/bin/bash
#SBATCH -o main.out
#SBATCH -N 1
#SBATCH -n 1
#SBATCH --mem=4G
#SBATCH -p defq

/home/jeremie.humeau/private/gitProject/csdvp-evolutionary-algorithm-optimization/build/application/ceao2 -G=10000 -P=100 -S=1 -C=105 -c=104 -R=7 -Q=5 -A=7 --outputfile=./TFtest -n=10 -N=25 -r=1 -q=0 -B=20 -x=0.75 -X=0.75 -F=3 -T=10


exit 0
