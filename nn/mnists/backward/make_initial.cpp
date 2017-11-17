#include <list>
#include <vector>
#include <iostream>
#include "util.h"

int main(){
	//w01[784][1024]
	float w01[N_INPUT][N_H1];
	//b1[1024]
	float b1[N_H1];

	//w12[1024][1024]
	float w12[N_H1][N_H2];
	//b2[1024]
	float b2[N_H2];

	//w23[1024][10]
	float w23[N_H1][N_OUTPUT];
	//b3[10]
	float b3[N_OUTPUT];

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

