set terminal postscript eps enhanced color "Helvetica" 22
set output "trianglecount.eps"

set title "Triangle Count Comparison"

set style fill solid 1.00 border rgb "#000000"
set style histogram errorbars gap 2 lw 1
set style data histogram
#set xlabel "Number of Triangles"
set xtics font ", 15"
#set xtics rotate by -30

set grid ytics
set ylabel "Elapsed Time In ms"
set yrange [0:*]

set key right outside

set datafile separator ","
#plot 'trianglecount.txt' using 2:3:xtic(1) ti "Primary Ray" lt 1 lc rgb "#d33682", \
#'' using 4:5 ti "Intersection" lt 1 lc rgb "#2aa198", \
#'' using 6:7 ti "Color" lt 1 lc rgb "#268bd2"

set key horiz
set key bot center
plot 'trianglecount2.txt' using 2:3:xtic(1) ti "24" lt 1 lc rgb "#d33682", \
'' using 4:5 ti "60" lt 1 lc rgb "#2aa198", \
'' using 6:7 ti "204" lt 1 lc rgb "#268bd2"
