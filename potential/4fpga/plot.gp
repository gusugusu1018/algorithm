set xlabel "x"
set ylabel "y"
set zlabel "potential"
splot 'potential.data' with lines title 'potential',\
	'path.data' with lines lw 4 title 'path'
