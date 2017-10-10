all:
	g++ main.cpp -o main.out -std=c++11 -lm
clean:
	rm main.out all.txt
plot:
	gnuplot -e "plot 'all.txt' using 1:2 title 'raw' with lines,\
		'all.txt' using 1:3 title '1000' with lines,\
		'all.txt' using 1:4 title '10000' with lines, \
		'all.txt' using 1:5 title '100000' with lines, \
		'all.txt' using 1:6 title '1000000' with lines, \
		'all.txt' using 1:7 title '10000000' with lines, \
		; pause -1"

