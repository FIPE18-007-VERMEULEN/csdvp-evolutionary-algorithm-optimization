#!/bin/bash


for nbCours in 15
do
    for nbComps in 10 20
    do
	for cbyTF in 2
	do
	    RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}"
	    mkdir $RESDIR	    
	    for run in 5
	    do
	    	RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}"
		sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR job.sh
	    done
	done
    done
done

for nbCours in 25
do
    for nbComps in 10
    do
	for cbyTF in 3
	do
	    RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}"
	    mkdir $RESDIR
	    for run in 5
	    do
		RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}"
		sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR job.sh
	    done
	done
    done
done

for nbCours in 20
do
    for nbComps in 20
    do
	for cbyTF in 3
	do
	    RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}"
	    mkdir $RESDIR
	    for run in 5
	    do
		RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}"
		sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR job.sh
	    done
	done
    done
done

for nbCours in 30
do
    for nbComps in 40
    do
	for cbyTF in 4
	do
	    RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}"
	    mkdir $RESDIR
	    for run in 5
	    do
		RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}"
		sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR job.sh
	    done
	done
    done
done

exit 0
