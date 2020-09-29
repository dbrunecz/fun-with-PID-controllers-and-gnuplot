CFLAGS += -Wall -Werror
CFLAGS += -ggdb

LDLIBS += -lm

#all: grav ctrl fir
all: grav ctrl

ctrl: ctrl.o noise.o filter.o

fir: fir.o dft.o

fir_plot: fir
	./fir > /tmp/fir.txt
	gnuplot -p -e "DATFILE='/tmp/fir.txt'" plot_sig.p


clean:
	@rm -f grav ctrl dft noise *.o 2&>/dev/null
