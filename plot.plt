#!/usr/bin/env gnuplot

set terminal pdfcairo enhanced dashed color size 4,4 font "Inconsolata,12"

set output 'fft.pdf'

set multiplot layout 2,1

set title 'Before FFT'
plot 'in1.out' notitle

set title 'After FFT'
plot 'out2.out' u 1:(($2**2+$3**2)**0.5) notitle lc rgb 'green'

unset multiplot
