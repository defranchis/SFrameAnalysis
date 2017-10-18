
#!/bin/bash

# g++ -o exe_lowStat_UNBLIND lowStat_UNBLIND.cpp `root-config --cflags --libs` &
# g++ -o exe_lowStat_BLIND lowStat_BLIND.cpp `root-config --cflags --libs` &
# g++ -o exe_highStat_BLIND highStat_BLIND.cpp `root-config --cflags --libs` &
# g++ -o exe_highStat_UNBLIND highStat_UNBLIND.cpp `root-config --cflags --libs` &
g++ -o exe_highStat_SEMIBLIND highStat_SEMIBLIND.cpp `root-config --cflags --libs` &
g++ -o exe_highStat_SR highStat_SR_lin.cpp `root-config --cflags --libs` &
# g++ -o exe_2D plots2D.cpp `root-config --cflags --libs` &

wait
