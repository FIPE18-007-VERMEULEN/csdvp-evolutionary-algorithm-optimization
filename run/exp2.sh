for idx in 1 2 3 4
do
    for nbCours in 105 #LATER 95 85 75 65 55 45 35
    do
	for i in 3 4 5
	do
	    for cbyTF in 5 6 7 8 9 10
	    do
		for maxCbyC in 7 #LATER 3 4 5 6 
		do
		    for maxPre in 2 #LATER 3 4 DONE 5
		    do
			for nbGen in 10000
			do
			    let "nbComps=$idx * ($nbCours/2)"
			    let "maxCbyTF=$nbCours/$i"
			    let "minCbyTF=$nbCours/10"
			    if [ $minCbyTF -lt $cbyTF ]
			    then
				let "minCbyTF=$cbyTF"
			    fi			
			    RESDIR="../results/${nbCours}_${nbComps}_${maxCbyC}_${maxPre}_${cbyTF}_${minCbyTF}_${maxCbyTF}_${nbGen}"
			    mkdir $RESDIR	    
			    for run in 30
			    do
				let "nbComps=$idx * ($nbCours/2)"
				let "maxCbyTF=$nbCours/$i"
				let "minCbyTF=$nbCours/10"
				if [ $minCbyTF -lt $cbyTF ]
				then
				    let "minCbyTF=$cbyTF"
				fi
				RESDIR="../results/${nbCours}_${nbComps}_${maxCbyC}_${maxPre}_${cbyTF}_${minCbyTF}_${maxCbyTF}_${nbGen}"
				
				sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,R=$maxCbyC,Q=$maxPre,A=$cbyTF,n=$minCbyTF,N=$maxCbyTF,r=1,q=0,O=$RESDIR,B=20,x=0.75,X=0.75,F=3,G=$nbGen job.sh
				echo "-C=${nbCours} -c=${nbComps} -R=${maxCbyC} -Q=${maxPre} -A=${cbyTF} -n=${minCbyTF} -N=${maxCbyTF} -r=1 -q=0" >> case
			    done
			done
		    done
		done
	    done
	done
    done
done
