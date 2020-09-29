#!/bin/bash
make dft
./dft > dft.txt
gnuplot -p -e "DATFILE='dft.txt'" plot_sig.p
