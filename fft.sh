#!/bin/bash

#sudo apt install gnuplot-x11

make fft
./fft > fft.txt
gnuplot -p -e "DATFILE='fft.txt'" plot_sig.p
