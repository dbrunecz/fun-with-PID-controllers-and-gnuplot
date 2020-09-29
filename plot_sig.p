set xlabel "Time, seconds" tc rgb "#f0f0f0"
set ylabel "Amplitude" tc rgb "#f0f0f0"

set xrange [*:*]
set yrange [*:*]

set terminal wxt size 1600,500

set object 1 rectangle from screen 0,0 to screen 1,1 fillcolor rgb"black" behind

set style line 1 lw 0.5 lc rgb "#e01010"

set style line 2 lw 0.4 lc rgb "#9fff9f"
set style line 3 lw 0.4 lc rgb "#9f9fff"
set style line 4 lw 0.4 lc rgb "#ff9898"
set style line 5 lw 0.4 lc rgb "#ffff30"

set style line 6 lw 0.2 lc rgb "#808080"

set grid back ls 6
set border back ls 6

plot \
DATFILE u 1:2 w lines ls 4

# pause -1

#gnuplot -p -e "DATFILE='/tmp/g.dat'" plot_sv.p
