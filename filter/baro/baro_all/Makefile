all:
	g++ main.cpp -o main.out -std=c++11 -lm
clean:
	rm main.out rawFchanged.txt filtered_baro_alt.txt filtered_baro_vel.txt filtered_baro_acc.txt
plot:
	gnuplot -e "load 'plot.gp'; pause -1"

