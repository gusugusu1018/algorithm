all:
	g++ main.cpp -o main.out -std=c++11 -lm
clean:
	rm main.out rawdata.txt lpf.txt kalman.txt all.txt
plot:
	gnuplot -e "plot 'all.txt' using 1:2 title 'raw' with lines,'all.txt' using 1:3 title 'lpf' with lines,'all.txt' using 1:4 title 'kalman' with lines; pause -1"
