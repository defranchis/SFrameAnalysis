
#!bin/bash

echo estimating systematics
./exe_syst

echo creating datacards
./exe_datacards

echo latex table
./exe_latex

