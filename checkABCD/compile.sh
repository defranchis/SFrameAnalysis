
#!/bin/bash

g++ -o exe_bkg binnedTTintegral.cpp `root-config --cflags --libs`&
# g++ -o exe_sig binnedRadionIntegral.cpp `root-config --cflags --libs`&
g++ -o exe_ABCD estimateBkg.cpp `root-config --cflags --libs`&
# g++ -o exe_datacards improvedDatacards.cpp `root-config --cflags --libs`&

wait