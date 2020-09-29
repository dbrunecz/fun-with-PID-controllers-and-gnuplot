#!/bin/bash

CF=/tmp/ctrl-`date +%H%M%S`.txt

make && ./ctrl > $CF

echo $CF
gnuplot -p -e "DATFILE='$CF'" plot_sv.p
#vi $CF
