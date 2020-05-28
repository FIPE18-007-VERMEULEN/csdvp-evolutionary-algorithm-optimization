#!/bin/bash


for nbCours in 15
do
    for nbComps in 10 20
    do
	for cbyTF in 2
	do
	    RESDIR="${nbCours}_${nbComps}_${cbyTF}"
	    mkdir $RESDIR
	    for run in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
	    do
		RESDIR="${nbCours}_${nbComps}_${cbyTF}"
#		sbatch --job-name=${RESDIR}_${run} --output=${RESDIR}_${run}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,S=$run job.sh
		sbatch --job-name=${RESDIR}_${run} --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,S=$run job.sh
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
	    RESDIR="${nbCours}_${nbComps}_${cbyTF}"
	    mkdir $RESDIR
	    for run in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
	    do
		RESDIR="${nbCours}_${nbComps}_${cbyTF}"
#		sbatch --job-name=${RESDIR}_${run} --output=${RESDIR}_${run}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,S=$run job.sh
		sbatch --job-name=${RESDIR}_${run} --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,S=$run job.sh
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
	    RESDIR="${nbCours}_${nbComps}_${cbyTF}"
	    mkdir $RESDIR
	    for run in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
	    do
		RESDIR="${nbCours}_${nbComps}_${cbyTF}"
#		sbatch --job-name=${RESDIR}_${run} --output=${RESDIR}_${run}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,S=$run job.sh
		sbatch --job-name=${RESDIR}_${run} --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,S=$run job.sh
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
	    RESDIR="${nbCours}_${nbComps}_${cbyTF}"
	    mkdir $RESDIR
	    for run in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
	    do
		RESDIR="${nbCours}_${nbComps}_${cbyTF}"
		sbatch --job-name=${RESDIR}_${run} --output=${RESDIR}_${run}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,S=$run job.sh
#		sbatch --job-name=${RESDIR}_${run} --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,S=$run job.sh
	    done
	done
    done
done


exit 0
