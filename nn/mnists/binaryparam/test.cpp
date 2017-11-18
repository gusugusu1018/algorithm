#include <list>
#include <vector>
#include <iostream>
#include <cmath>
#include "mnist.h"
#include "util.h"

float relu(float a) {
	return a > 0.0f ? a:0.0f;
}

float diff_relu(float a) {
	return a > 0.0f ? 1.0f:0.0f;
}

int main(){
	int epoch = 60400;
	float eta = 0.1f;

	std::cout << "init dataset..." << std::endl;
	std::vector<std::vector<float> > train_data;
	std::vector<float> label_data;
	Mnist mnist;
	train_data = mnist.readTrainingFile("../data/train-images-idx3-ubyte");
	label_data = mnist.readLabelFile("../data/train-labels-idx1-ubyte");

	float data[N_INPUT];

	float w01[N_INPUT][N_H1];
	float b1[N_H1];
	float z1[N_H1];
	float a1[N_H1];
	float delta_1[N_H1];

	float w12[N_H1][N_H2];
	float b2[N_H2];
	float z2[N_H2];
	float a2[N_H2];
	float delta_2[N_H2];

	float w23[N_H1][N_OUTPUT];
	float b3[N_OUTPUT];
	float z3[N_OUTPUT];
	float a3[N_OUTPUT];
	float delta_3[N_OUTPUT];

	float teacher_buf[10];

	float max = 0.0f;
	float sum = 0.0f;
	float cost = 0.0f;

	//****************initialize weight and bias*******************
	for (int i=0;i<N_INPUT;i++) {
		for (int j=0;j<N_H1;j++)
			w01[i][j] = 0.1f;
	}
	for (int i=0;i<N_H1;i++) {
		b1[i] = -1.0f;
	}
	for (int i=0;i<N_H1;i++) {
		for (int j=0;j<N_H2;j++)
			w12[i][j] = 0.1f;
	}
	for (int i=0;i<N_H2;i++) {
		b2[i] = -1.0f;
	}
	for (int i=0;i<N_H2;i++) {
		for (int j=0;j<N_OUTPUT;j++)
			w23[i][j] = 0.1f;
	}
	for (int i=0;i<N_OUTPUT;i++) {
		b3[i] = -1.0f;
	}

	write_weights(w01,b1,w12,b2,w23,b3);

	return 0;
}

