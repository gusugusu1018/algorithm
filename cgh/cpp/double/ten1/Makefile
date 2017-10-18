all:
	g++ -o main.out main.cpp -O3 -lm -g -std=c++11
clean:
	rm cgh.out cgh.bmp dump.txt
show:
	eog cgh.bmp
dump:
	objdump -dS cgh.out > dump.txt
	less dump.txt
