all:
	g++ main.cpp -o main.out -std=c++11 -lm
clean:
	rm main.out all.txt
plot:
	gnuplot -e "plot 'all.txt' using 1:2 title 'raw' with lines,\
		'all.txt' using 1:3 title '0.0001' with lines,\
		'all.txt' using 1:4 title '0.001' with lines, \
		'all.txt' using 1:5 title '0.01' with lines, \
		'all.txt' using 1:6 title '0.1' with lines, \
		'all.txt' using 1:7 title '1' with lines, \
		; pause -1"
