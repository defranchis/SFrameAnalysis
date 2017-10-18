
#!bin/bash

# DIRNAME=01_massWindow_CR
DIRNAME=02_controlPlots

for DIR in rootFiles txtFiles outputs; do

    if [ ! -e $DIR/$DIRNAME ]; then
        mkdir $DIR/$DIRNAME
    fi

done

printf "\n"
echo compiling
printf "\n"
source compile.sh
wait 
printf "\n"



printf "\n"
echo $DIRNAME

# echo estimating signal
# ./exe_sig $DIRNAME
echo preparing to estimate background
./exe_bkg $DIRNAME > outputs/$DIRNAME/outBkg.txt
# echo estimating backgrounds
# ./exe_ABCD $DIRNAME > outputs/$DIRNAME/usedTF.txt

# echo estimating systematic uncertainties
# ./exe_syst

# echo creating datacards
# ./exe_datacards $DIRNAME

printf "\n"
echo directory $DIRNAME: done!
printf "\n"






