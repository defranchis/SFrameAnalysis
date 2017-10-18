#!/bin/bash

echo Enter directory name and press [ENTER]
read DIRNAME

for OUTDIR in plot; do
    
    if [ ! -e $OUTDIR/$DIRNAME ]; then
        mkdir $OUTDIR/$DIRNAME
    fi
    
done


printf "\n"
echo copying datacards

cp -r ~/HH/8_X_Framework/Analysis/SFrameAnalysis/newDatacards/datacards/$DIRNAME/*.txt datacards

printf "\n"
echo compiling
source compile.sh

echo running datacards
printf "\n"
source runDatacards.sh
wait

echo making plot
./exe_plot $DIRNAME
printf "\n"
echo done!
printf "\n"
