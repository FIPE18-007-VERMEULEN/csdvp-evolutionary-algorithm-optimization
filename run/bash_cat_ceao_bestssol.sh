#bash

for d in */ ;
do
    echo $d" " | tr -d "\n" >> 'allallbestsols.txt'
    cd $d
    rm -f 'allbestsols.txt'

    for f in bestsol.* ;
    do
        cat $f >> 'allbestsols.txt'
        echo >> 'allbestsols.txt'

        cat $f | cut -d " " -f 1 | tr -d "\n" >> ../'allallbestsols.txt'
        echo " " | tr -d "\n" >> ../'allallbestsols.txt'
    done
    cd ..
    echo >> 'allallbestsols.txt'

done