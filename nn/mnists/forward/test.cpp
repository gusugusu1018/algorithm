#include <list>
#include <vector>
#include <iostream>
#include <cmath>
#include "mnist.h"
#include "util.h"

float relu(float a) {
	return a > 0.0f ? a:0.0f;
}

int main(){
	std::cout << "init dataset..." << std::endl;
	std::vector<std::vector<float> > train_data;
	std::vector<float> label_data;
	Mnist mnist;
	train_data = mnist.readTrainingFile("../data/train-images-idx3-ubyte");
	label_data = mnist.readLabelFile("../data/train-labels-idx1-ubyte");

	//data[784]
	float data[N_INPUT];

	//w01[784][1024]
	float w01[N_INPUT][N_H1];
	//b1[1024]
	float b1[N_H1];
	//z1[1024]
	float z1[N_H1];
	//a1[1024]
	float a1[N_H1];

	//w12[1024][1024]
	float w12[N_H1][N_H2];
	//b2[1024]
	float b2[N_H2];
	//z2[1024]
	float z2[N_H2];
	//a2[1024]
	float a2[N_H2];

	//w23[1024][10]
	float w23[N_H1][N_OUTPUT];
	//b3[10]
	float b3[N_OUTPUT];
	//z3[10]
	float z3[N_OUTPUT];
	//a3[10]
	float a3[N_OUTPUT];

	//*************************initialize****************************
	for (int i=0;i<N_INPUT;i++) {
		for (int j=0;j<N_H1;j++)
			w01[i][j] = 0.1f;
	}
	for (int i=0;i<N_H1;i++) {
		b1[i] = -1.0f;
		z1[i] = 0.0f;
		a1[i] = 0.0f;
	}
	for (int i=0;i<N_H1;i++) {
		for (int j=0;j<N_H2;j++)
			w12[i][j] = 0.1f;
	}
	for (int i=0;i<N_H2;i++) {
		b2[i] = -1.0f;
		z2[i] = 0.0f;
		a2[i] = 0.0f;
	}
	for (int i=0;i<N_H2;i++) {
		for (int j=0;j<N_OUTPUT;j++)
			w23[i][j] = 0.1f;
	}
	for (int i=0;i<N_OUTPUT;i++) {
		b3[i] = -1.0f;
		z3[i] = 0.0f;
		a3[i] = 0.0f;
	}
	//*************************forward****************************
	int k=0;
	// standlize
	for (int j=0;j<28;j++) {
		for (int i=0;i<28;i++) {
			data[i+j*28] = train_data[k][i+j*28]/255;
		}
	}

	// H1 perseptron 1024kai
	for (int j=0;j<N_H1;j++) {
		// perseptron 784->1
		for (int i=0;i<N_INPUT;i++) {
			// sum w*data
			z1[j]+=w01[i][j]*data[i];
		}
		// sum bias
		z1[j]+=b1[j];
		// relu function
		a1[j]=relu(z1[j]);
	}

	// H2 perseptron 1024kai
	for (int j=0;j<N_H2;j++) {
		// perseptron 1024->1
		for (int i=0;i<N_H1;i++) {
			// sum w*a1
			z2[j]+=w12[i][j]*a1[i];
		}
		// sum bias
		z2[j]+=b2[j];
		// relu function
		a2[j]=relu(z2[j]);
	}

	// OUTPUT perseptron 10kai
	for (int j=0;j<N_OUTPUT;j++) {
		// perseptron 1024->1
		for (int i=0;i<N_H2;i++) {
			// sum w*a1
			z3[j]+=w23[i][j]*a2[i];
		}
		// sum bias
		z3[j]+=b3[j];
	}
	//softmax
	float max = 0.0f;
	float sum = 0.0f;
	for (int i=0; i<N_OUTPUT; i++) if(max < z3[i]) max = z3[i];
	for (int i=0; i<N_OUTPUT; i++) {
		a3[i] = exp(z3[i] - max);
		sum += a3[i];
	}
	for (int i=0; i<N_OUTPUT; i++) a3[i] /= sum;

	// convert teacher to one-hot expression
	// if (label==0) teacher_buf={1,0,0,0,0,0,0,0,0,0};
	// if (label==1) teacher_buf={0,1,0,0,0,0,0,0,0,0};
	// if (label==2) teacher_buf={0,0,1,0,0,0,0,0,0,0};
	// if (label==3) teacher_buf={0,0,0,1,0,0,0,0,0,0};
	//
	// if (label==9) teacher_buf={0,0,0,0,0,0,0,0,0,1};
	float teacher_buf[10];
	for (int i=0;i<10;i++) {
		if (label_data[k] == i) {
			teacher_buf[i]=1;
		} else {
			teacher_buf[i]=0;
		}
	}
	float cost = 0.0f;
	// cross entropy error
	for (int i=0;i<10;i++) {
		cost += -teacher_buf[i]*log(a3[i]+1e-8);
	}
	std::cout<<"cost "<<cost<<std::endl;

	return 0;
}

