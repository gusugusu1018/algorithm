all:
	gcc potential-path-planning.c -O3
#icc potential-path-planning.c -O3
clean:
	rm a.out *.data
plot:
	gnuplot -e "load 'plot.gp'; pause -1"
