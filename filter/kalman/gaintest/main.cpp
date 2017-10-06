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
	double P, K;
	double X;
public:
	double Q;// = 0.0001;
	double R;// = 0.01;
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
	SimpleKalman sk1;
	SimpleKalman sk2;
	SimpleKalman sk3;
	SimpleKalman sk4;
	SimpleKalman sk5;

	sk1.Q = 0.0001;
	sk2.Q = 0.0001;
	sk3.Q = 0.0001;
	sk4.Q = 0.0001;
	sk5.Q = 0.0001;
	sk1.R = 0.0001;
	sk2.R = 0.001;
	sk3.R = 0.01;
	sk4.R = 0.1;
	sk5.R = 1.0;

	std::ofstream ofs_all("all.txt");

	srand((unsigned)time(NULL));
	double *dat = genGaussArray(num, 10.0, 0.4);

	// Simple Kalman Filter
	for (int i = 0; i < num; i++)
	{
		if (i == 0) {
			ofs_all << i << "\t" << dat[i] << "\t" << sk1.initialize(dat[i]) << "\t" << sk2.initialize(dat[i]) << "\t" << sk3.initialize(dat[i]) << "\t" << sk4.initialize(dat[i]) << sk5.initialize(dat[i]) << std::endl;
		} else {
			ofs_all << i << "\t" << dat[i] << "\t" << sk1.update(dat[i]) << "\t" << sk2.update(dat[i]) << "\t" << sk3.update(dat[i]) << "\t" << sk4.update(dat[i]) << "\t" << sk5.update(dat[i]) << std::endl;
		} 
	}
	free(dat);

	return 0;
}
