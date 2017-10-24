all:
	nvcc -Wno-deprecated-gpu-targets -o main.out main.cu
clean:
	rm main.out cgh.bmp dump.txt
show:
	eog cgh.bmp
dump:
	objdump -dS cgh.out > dump.txt
	less dump.txt
