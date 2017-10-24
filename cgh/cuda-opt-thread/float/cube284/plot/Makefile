all:
	g++ main.cpp -o main.out -std=c++11 -lm
clean:
	rm main.out plot.txt
plot:
	gnuplot -e "load 'plot.gp'; pause -1"

