#!/bin/bash
gnuplot -p -e "DATFILE='${1}'" plot_sig.p
