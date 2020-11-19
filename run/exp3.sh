#
for nbCours in 55 #LATER 95 85 75 65 55 45 35
do
    for i in 3 #4 5
    do
	for cbyTF in 6 #6 7 8 9 10
	do
	    for maxCbyC in 5 #LATER 3 4 5 6 
	    do
		for maxPre in 3 #LATER 2 3 4
		do
		    for nbGen in 10000
		    do
			nbComps=75
			RESDIR="../results/${nbCours}_${nbComps}_${maxCbyC}_${maxPre}_${cbyTF}_${nbGen}"
			mkdir $RESDIR	    
			for run in 30
			do
			    nbComps=75
			    minCbyTF=7
			    maxCbyTF=15
			    RESDIR="../results/${nbCours}_${nbComps}_${maxCbyC}_${maxPre}_${cbyTF}_${nbGen}"
			    
			    sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,R=$maxCbyC,Q=$maxPre,A=$cbyTF,n=$minCbyTF,N=$maxCbyTF,r=1,q=0,O=$RESDIR,B=50,x=0.75,X=0.75,F=3,G=$nbGen job.sh
			    echo "-C=${nbCours} -c=${nbComps} -R=${maxCbyC} -Q=${maxPre} -A=${cbyTF} -n=${minCbyTF} -N=${maxCbyTF} -r=1 -q=0" >> case
			done
		    done
		done
	    done
	done
    done
done
