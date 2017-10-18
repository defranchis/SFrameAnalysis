
#!/bin/bash

g++ -o exe_bkg bkgIntegral.cpp `root-config --cflags --libs`&
g++ -o exe_sig radionIntegral.cpp `root-config --cflags --libs`&
g++ -o exe_ABCD estimateBkg.cpp `root-config --cflags --libs`&
g++ -o exe_datacards createDatacards.cpp `root-config --cflags --libs`&
g++ -o exe_syst estimateSyst.cpp `root-config --cflags --libs`&
g++ -o exe_latex latex.cpp `root-config --cflags --libs`&

wait