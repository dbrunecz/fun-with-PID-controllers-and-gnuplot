#!/bin/bash

#sudo apt install gnuplot-x11

make dft
./dft > dft.txt
gnuplot -p -e "DATFILE='dft.txt'" plot_sig.p
