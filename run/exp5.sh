#
for nbCours in 400 #LATER 300 310 320 330 340 350 360 370 380 390 410 420 430 440 450 460 470 480 490 500
do
    for cbyTF in 15
    do
	for maxCbyC in 3
	do
	    for maxPre in 3 
	    do
		for nbGen in 10000
		do
		    nbComps=400
		    RESDIR="../results/${nbCours}_${nbComps}_${maxCbyC}_${maxPre}_${cbyTF}_${nbGen}"
		    mkdir $RESDIR	    
		    for run in 1
		    do
			nbComps=400
			minCbyTF=16
			maxCbyTF=40
			RESDIR="../results/${nbCours}_${nbComps}_${maxCbyC}_${maxPre}_${cbyTF}_${nbGen}"
			
			sbatch --array=1-${run} --job-name=${RESDIR} --output=${RESDIR}.out --export=C=$nbCours,c=$nbComps,R=$maxCbyC,Q=$maxPre,A=$cbyTF,n=$minCbyTF,N=$maxCbyTF,r=1,q=0,O=$RESDIR,B=50,x=0.75,X=0.75,F=3,G=$nbGen job.sh
			echo "-C=${nbCours} -c=${nbComps} -R=${maxCbyC} -Q=${maxPre} -A=${cbyTF} -n=${minCbyTF} -N=${maxCbyTF} -r=1 -q=0" >> case
		    done
		done
	    done
	done
    done
done
