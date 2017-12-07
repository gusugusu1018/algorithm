all:
	nvcc -Wno-deprecated-gpu-targets potential-path-planning.cu
#icc potential-path-planning.c -O3
clean:
	rm a.out *.data
plot:
	gnuplot -e "load 'plot.gp'; pause -1"
