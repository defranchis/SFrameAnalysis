
#!/bin/bash

g++ -o exe_plot makePlot.cpp `root-config --cflags --libs` &
g++ -o exe_script scriptCreator.cpp `root-config --cflags --libs` &

wait


