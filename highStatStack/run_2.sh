
#!bin/bash

./exe_lowStat_BLIND new_SR > outputs/new_SR.txt &
./exe_lowStat_UNBLIND new_bveto > outputs/new_bveto.txt &
./exe_lowStat_UNBLIND new_tauveto > outputs/new_tauveto.txt &
./exe_lowStat_UNBLIND new_vetoall > outputs/new_vetoall.txt &
