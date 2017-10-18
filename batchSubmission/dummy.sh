#!bin/bash

DIR=/pnfs/psi.ch/cms/trivcat/store/user/clange/Ntuple_8020_20170112/JetHT
COUNTER=0

for SAMPLE in $( ls $DIR ); do

    let "COUNTER++"
    echo $DIR/$SAMPLE > samples/data_$COUNTER.txt
    python createXMLfile.py -m 450 -o xmls_17_02_01 samples/data_$COUNTER.txt &

done

