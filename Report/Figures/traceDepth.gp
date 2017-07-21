
#set terminal svg size 512,512 fname "Verdana" fsize 10
#set output "barchart.svg"

set terminal postscript eps enhanced color "Helvetica" 22
set output "traceDepth.eps"

#set terminal png size 1024,512 enhanced font "Verdana,20"
#set output 'resolution.png'

set title "Trace Depth Comparison"

set style fill solid 1.00 border rgb "#000000"
set style histogram errorbars gap 2 lw 1
set style data histogram
#set xlabel "Trace Depth"
set xtics font ", 15"
#set xtics rotate by -30

set grid ytics
set ylabel "Elapsed Time In ms"
set yrange [0:*]

set key right outside

set datafile separator ","
#plot 'traceDepth.txt' using 2:3:xtic(1) ti "Primary Ray" lt 1 lc rgb "#d33682", \
#'' using 4:5 ti "Intersection" lt 1 lc rgb "#2aa198", \
#'' using 6:7 ti "Color" lt 1 lc rgb "#268bd2"

set key horiz
set key bot center
plot 'traceDepth2.txt' using 2:3:xtic(1) ti "2" lt 1 lc rgb "#d33682", \
'' using 4:5 ti "3" lt 1 lc rgb "#2aa198", \
'' using 6:7 ti "4" lt 1 lc rgb "#268bd2"
