all:
	g++ main.cpp -o main.out -std=c++11 -lm
clean:
	rm main.out rawFchanged.txt filtered_baro_alt.txt filtered_baro_vel.txt filtered_baro_acc.txt
plot:
	gnuplot -e "plot '1.txt' with lines; pause -1"

