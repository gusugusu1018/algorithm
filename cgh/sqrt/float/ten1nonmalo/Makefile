all:
	gcc -o cgh.out cgh.c -O3 -lm
clean:
	rm cgh.out cgh.bmp dump.txt
show:
	eog cgh.bmp
dump:
	readelf -h cgh.out
	objdump -dS cgh.out > dump.txt
	less dump.txt
