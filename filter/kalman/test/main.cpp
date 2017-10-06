#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

class SimpleKalman
{
	constexpr static double Q = 0.0001;
	constexpr static double R = 0.01;
	double P, K;
	double X;
public:
	SimpleKalman()
		:P(0.0), X(0.0)
		 {};

	double initialize(double first_value)
	{
		X = first_value;
	}

	double update(double measurement)
	{
		measurementUpdate();
		double result = X + (measurement - X) * K;
		X = result;
		return result;
	}

private:
	void measurementUpdate()
	{
		K = (P + Q) / (P + Q + R);
		P = R * (P + Q) / (R + P + Q);
	}
};

class SimpleLPF
{
	const static int win_size = 16;
	double data[win_size];
	double total;
	int pointer;
public:
	SimpleLPF()
	:total(0.0), pointer(0)
	{
		for(int i=0;i<win_size;i++) data[i] = 0.0;
	};

	double update(double measurement)
	{
		total -= data[pointer];
		total += measurement;
		data[pointer] = measurement;
		pointer++;
		if(pointer==win_size) pointer = 0;
		return total/win_size;	// total>>4;
	}
};

class SimpleHPF
{
	SimpleLPF lpf;
public:
	SimpleHPF(){};

	double update(double measurement)
	{
		return measurement - lpf.update(measurement);
	}
};

double *genGaussArray(int size, double myu, double sigma)
{
	double *a = (double *)malloc(sizeof(double)*size);
	if(!a){
		printf("There is not an enough memory!\n");
		exit(1);
	}

	for(int i=0;i<size;i+=2){
		int rnd0 = 0, rnd1 = 0;
		while(0==rnd0) rnd0 = rand();
		while(0==rnd1) rnd1 = rand();
		double tmp0 = (double)rnd0/RAND_MAX;
		double tmp1 = (double)rnd1/RAND_MAX;
		a[i] = myu + sigma*sqrt(-2*log(tmp0))*sin(2*M_PI*tmp1);
		if(i+1<size)
			a[i+1] = myu + sigma*sqrt(-2*log(tmp0))*cos(2*M_PI*tmp1);
	}
	return a;
}

int main(int argc, char **argv)
{
	const static int num = 256;
	SimpleKalman sk;
	SimpleLPF slpf;
	SimpleHPF shpf;

	std::ofstream ofs_rawdata("rawdata.txt");
	std::ofstream ofs_kalman("kalman.txt");
	std::ofstream ofs_lpf("lpf.txt");
	std::ofstream ofs_all("all.txt");

	srand((unsigned)time(NULL));
	double *dat = genGaussArray(num, 10.0, 0.4);

	// Check Gaussian distribution array
	for (int i = 0; i < num; i++)
	{
		ofs_rawdata << i << "\t" << dat[i] << std::endl;
	}
	// Simple Low-pass Filter
	for (int i = 0; i < num; i++)
	{
		ofs_lpf << i << "\t" << slpf.update(dat[i]) << std::endl;
	}
	// Simple Kalman Filter
	for (int i = 0; i < num; i++)
	{
		if (i == 0) {
			ofs_kalman << i << "\t" << sk.initialize(dat[i]) << std::endl;
		} else {
			ofs_kalman << i << "\t" << sk.update(dat[i]) << std::endl;
		} 
	}
	// all
	for (int i = 0; i < num; i++)
	{
		if (i == 0) {
			ofs_all << i << "\t" << dat[i] << "\t" << slpf.update(dat[i]) << "\t" << sk.initialize(dat[i]) << std::endl;
		} else {
			ofs_all << i << "\t" << dat[i] << "\t" << slpf.update(dat[i]) << "\t" << sk.update(dat[i]) << std::endl;
		} 
	}

/*
	printf("Simple High-pass Filter, ");
	for (int i = 0; i < num; i++)
	{
		printf("%.2f, ", shpf.update(dat[i]));
	}
	printf("\n");
*/
	free(dat);

	return 0;
}
