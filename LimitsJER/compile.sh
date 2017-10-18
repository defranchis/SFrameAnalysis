
#!/bin/bash

g++ -o exe_plot makePlot.cpp `root-config --cflags --libs` &

wait


