
#!bin/bash

DIRNAME=checkABCD_v4
#inclusiveTT_noHiggsWindow_ABCDcheck
#ABCD_check
# DIRNAME=checkABCD_TTJetsHT_HiggsMass

printf "\n"
echo compiling
printf "\n"
source compile.sh
wait 
printf "\n"

# echo estimating signal
# ./exe_sig $DIRNAME
echo preparing to estimate background
./exe_bkg $DIRNAME > outputs/outBkg.txt
echo estimating backgrounds
./exe_ABCD > outputs/usedTF.txt

# echo creating datacards
# ./exe_datacards

printf "\n"
echo directory $DIRNAME: done!
printf "\n"