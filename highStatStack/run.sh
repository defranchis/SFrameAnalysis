
#!bin/bash

./exe_highStat_SEMIBLIND 00_vLoose > outputs/00_vLoose.txt &
./exe_lowStat_BLIND 01_SR > outputs/01_SR.txt &
./exe_lowStat_UNBLIND 02_bveto > outputs/02_bveto.txt &
./exe_lowStat_UNBLIND 03_antiIso > outputs/03_antiIso.txt &
./exe_lowStat_UNBLIND 04_invCuts > outputs/04_invCuts.txt &
