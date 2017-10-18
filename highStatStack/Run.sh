
#!bin/bash

source compile.sh
# ./exe_highStat_SR control_noPruned > outputs/control_noPruned.txt &
# ./exe_highStat_SEMIBLIND controlPlots > outputs/controlPlots.txt &
./exe_highStat_SEMIBLIND controlPlots &
wait