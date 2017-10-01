all:
	gcc -o cgh.out cgh.c -O3 -lm -g
clean:
	rm cgh.out cgh.bmp dump.txt
show:
	eog cgh.bmp
dump:
	objdump -dS cgh.out > dump.txt
	less dump.txt
