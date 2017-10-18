#!/bin/bash

DIR=datacards
COMBDIR=combinedDatacards
SUFFIX=".txt"
OUTPUT=newCombine.sh

if [ -e $OUTPUT ]; then
    rm $OUTPUT
fi


while read MASS; do

    PREFIX=$DIR/radion_M$MASS\_

    #if [ $MASS -eq "1000" ]; then # remove this line

    printf "python combineCards.py " >> $OUTPUT

    for CARD in $( ls $DIR/*$MASS* ); do

        BIN=${CARD%$SUFFIX}
        BIN=${BIN#$PREFIX}
        printf "bin$BIN=$CARD " >> $OUTPUT

    done
    
    PREFIX=${PREFIX%"_"}
    PREFIX=${PREFIX#$DIR"/"}
    
    printf " > $COMBDIR/$PREFIX$SUFFIX & \n\n" >> $OUTPUT
        
    #fi # remove this line

done < utility/massPoints.txt
