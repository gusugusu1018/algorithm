all:
	nvcc -Wno-deprecated-gpu-targets -o main.out main.cu -std=c++11
clean:
	rm main.out cgh.bmp timestamps.txt
show:
	eog cgh.bmp
dump:
	objdump -dS cgh.out > dump.txt
	less dump.txt
