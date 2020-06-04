for nbCours in 15
do
    for nbComps in 10 20
    do
	for cbyTF in 2
	do
	    for rEngine in 50
	    do
		for pMut in 0.25 0.5 0.75
		do
		    for pCross in 0.25 0.5 0.75
		    do
			for tSize in 3 7
			do
			    RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}_${rEngine}_${pMut}_${pCross}_${tSize}"
			    mkdir $RESDIR	    
			    for run in 30
			    do
	    			RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}_${rEngine}_${pMut}_${pCross}_${tSize}"
				sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,B=$rEngine,x=$pMut,X=$pCross,F=$tSize job.sh
			    done
			done
		    done
		done
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
	    for rEngine in 50
	    do
		for pMut in 0.25 0.5 0.75
		do
		    for pCross in 0.25 0.5 0.75
		    do
			for tSize in 3 7
			do
			    RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}_${rEngine}_${pMut}_${pCross}_${tSize}"
			    mkdir $RESDIR	    
			    for run in 30
			    do
	    			RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}_${rEngine}_${pMut}_${pCross}_${tSize}"
				sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,B=$rEngine,x=$pMut,X=$pCross,F=$tSize job.sh
			    done
			done
		    done
		done
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
	    for rEngine in 50
	    do
		for pMut in 0.25 0.5 0.75
		do
		    for pCross in 0.25 0.5 0.75
		    do
			for tSize in 3 7
			do
			    RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}_${rEngine}_${pMut}_${pCross}_${tSize}"
			    mkdir $RESDIR	    
			    for run in 30
			    do
	    			RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}_${rEngine}_${pMut}_${pCross}_${tSize}"
				sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,B=$rEngine,x=$pMut,X=$pCross,F=$tSize job.sh
			    done
			done
		    done
		done
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
	    for rEngine in 50
	    do
		for pMut in 0.25 0.5 0.75
		do
		    for pCross in 0.25 0.5 0.75
		    do
			for tSize in 3 7
			do
			    RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}_${rEngine}_${pMut}_${pCross}_${tSize}"
			    mkdir $RESDIR	    
			    for run in 30
			    do
	    			RESDIR="../results/${nbCours}_${nbComps}_${cbyTF}_${rEngine}_${pMut}_${pCross}_${tSize}"
				sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,A=$cbyTF,O=$RESDIR,B=$rEngine,x=$pMut,X=$pCross,F=$tSize job.sh
			    done
			done
		    done
		done
	    done
	done
    done
done
