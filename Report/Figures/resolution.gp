
#set terminal svg size 512,512 fname "Verdana" fsize 10
#set output "barchart.svg"

set terminal postscript eps enhanced color "Helvetica" 22
set output "resolution.eps"

#set terminal png size 1024,512 enhanced font "Verdana,20"
#set output 'resolution.png'

set title "Screen Resolution Comparison"

set style fill solid 1.00 border rgb "#000000"
set style histogram errorbars gap 2 lw 1
set style data histogram
#set xlabel "Resolution"
set xtics font ", 15"
#set xtics rotate by -30

set grid ytics
set ylabel "Elapsed Time In ms"
set yrange [0:*]

set key right outside

set datafile separator ","
#plot 'resolution.txt' using 2:3:xtic(1) ti "Primary Ray" lt 1 lc rgb "#d33682", \
#'' using 4:5 ti "Intersection" lt 1 lc rgb "#2aa198", \
#'' using 6:7 ti "Color" lt 1 lc rgb "#268bd2"

set key horiz
set key bot center
plot 'resolution2.txt' using 2:3:xtic(1) ti "800x600" lt 1 lc rgb "#d33682", \
'' using 4:5 ti "1280x1024" lt 1 lc rgb "#2aa198", \
'' using 6:7 ti "1920x1080" lt 1 lc rgb "#268bd2"
