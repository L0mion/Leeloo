set terminal postscript eps enhanced color "Helvetica" 22
set output "lightcount.eps"

set title "Light Count Comparison"

set style fill solid 1.00 border rgb "#000000"
set style histogram errorbars gap 2 lw 1
set style data histogram
#set xlabel "Light Count"
set xtics font ", 15"
#set xtics rotate by -30

set grid ytics
set ylabel "Elapsed Time In ms"
set yrange [0:*]

set key right outside

set datafile separator ","
#plot 'lightcount.txt' using 2:3:xtic(1) ti "Primary Ray" lt 1 lc rgb "#d33682", \
#'' using 4:5 ti "Intersection" lt 1 lc rgb "#2aa198", \
#'' using 6:7 ti "Color" lt 1 lc rgb "#268bd2"

set key horiz
set key bot center
plot 'lightcount2.txt' using 2:3:xtic(1) ti "2" lt 1 lc rgb "#d33682", \
'' using 4:5 ti "4" lt 1 lc rgb "#2aa198", \
'' using 6:7 ti "6" lt 1 lc rgb "#268bd2"
