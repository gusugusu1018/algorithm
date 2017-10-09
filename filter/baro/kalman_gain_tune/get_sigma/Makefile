all:
	g++ main.cpp -o main.out -std=c++11 -lm
clean:
	rm main.out all.txt
plot:
	gnuplot -e "plot 'all.txt' using 1:2 title 'raw' with lines,\
		'all.txt' using 1:3 title 'myu' with lines,\
		'all.txt' using 1:4 title 'sigma' with lines,\
		; pause -1"

