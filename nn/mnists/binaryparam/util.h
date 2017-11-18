#include <sstream>
#include <fstream>

#define N_INPUT 784
#define N_H1	1024
#define N_H2	1024
#define N_OUTPUT	10


int get_weights(float w01[N_INPUT][N_H1]
		,float b1[N_H1]
		,float w12[N_H1][N_H2]
		,float b2[N_H2]
		,float w23[N_H2][N_OUTPUT]
		,float b3[N_OUTPUT]
		) {
	return 1;
}

int write_weights(float w01[N_INPUT][N_H1]
		,float b1[N_H1]
		,float w12[N_H1][N_H2]
		,float b2[N_H2]
		,float w23[N_H2][N_OUTPUT]
		,float b3[N_OUTPUT]
		) {
	FILE *fp;
	if ((fp=fopen("test.weights","wb+"))==NULL) {
		printf("Can't open a file");
		return 1;
	}
	struct Param {
		float w01[N_INPUT*N_H1];
		float b1[N_H1];
		float w12[N_H1*N_H2];
		float b2[N_H2];
		float w23[N_H2*N_OUTPUT];
		float b3[N_OUTPUT];
	} weights;

	for (int i=0;i<N_INPUT;i++) {
		for (int j=0;j<N_H1;j++) {
			weights.w01[i*N_H1+j] = w01[i][j];
		}
	}
	for (int i=0;i<N_INPUT;i++) {
		for (int j=0;j<N_H1;j++) {
			weights.w01[i*N_H1+j] = w01[i][j];
		}
	}
	weights.b1 = *b1;
	weights.w12 = *w12;
	weights.b2 = *b2;
	weights.w23 = *w23;
	weights.b3 = *b3;

	fwrite(&weights,sizeof(weights),1,fp);
	fclose(fp);
	return 1;
}
