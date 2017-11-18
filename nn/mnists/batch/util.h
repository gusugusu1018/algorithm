struct Param {
	float w01[N_INPUT*N_H1];
	float b1[N_H1];
	float w12[N_H1*N_H2];
	float b2[N_H2];
	float w23[N_H2*N_OUTPUT];
	float b3[N_OUTPUT];
} weights;

int get_weights(
		char filename[MAX_FILENAME]
		,float w01[N_INPUT][N_H1]
		,float b1[N_H1]
		,float w12[N_H1][N_H2]
		,float b2[N_H2]
		,float w23[N_H2][N_OUTPUT]
		,float b3[N_OUTPUT]
	       ) {
	FILE *readfp;
	if ((readfp = fopen(filename, "rb")) == NULL) {
		printf("Can't open a file.");
		return 1;
	}
	int ret = (int)fread(&weights, sizeof(weights), 1, readfp);
	//printf("size %d\n",ret);
	for (int i=0;i<N_INPUT;i++) {
		for (int j=0;j<N_H1;j++) {
			w01[i][j] = weights.w01[i*N_H1+j];
		}
	}
	for (int i=0;i<N_H1;i++) {
		for (int j=0;j<N_H2;j++) {
			w12[i][j] = weights.w12[i*N_H2+j];
		}
	}
	for (int i=0;i<N_H2;i++) {
		for (int j=0;j<N_OUTPUT;j++) {
			w23[i][j] = weights.w23[i*N_OUTPUT+j];
		}
	}
	for (int j=0;j<N_H1;j++) {
		b1[j] = weights.b1[j];
	}
	for (int j=0;j<N_H2;j++) {
		b2[j] = weights.b2[j];
	}
	for (int j=0;j<N_OUTPUT;j++) {
		b3[j] = weights.b3[j];
	}
	fclose(readfp);
	return 0;
}

int write_weights(
		char filename[MAX_FILENAME]
		,float w01[N_INPUT][N_H1]
		,float b1[N_H1]
		,float w12[N_H1][N_H2]
		,float b2[N_H2]
		,float w23[N_H2][N_OUTPUT]
		,float b3[N_OUTPUT]
		) {
	FILE *writefp;
	if ((writefp=fopen(filename,"wb+"))==NULL) {
		printf("Can't open a file");
		return 1;
	}

	for (int i=0;i<N_INPUT;i++) {
		for (int j=0;j<N_H1;j++) {
			weights.w01[i*N_H1+j] = w01[i][j];
		}
	}
	for (int i=0;i<N_H1;i++) {
		for (int j=0;j<N_H2;j++) {
			weights.w12[i*N_H2+j] = w12[i][j];
		}
	}
	for (int i=0;i<N_H2;i++) {
		for (int j=0;j<N_OUTPUT;j++) {
			weights.w23[i*N_OUTPUT+j] = w23[i][j];
		}
	}
	for (int j=0;j<N_H1;j++) {
		weights.b1[j] = b1[j];
	}
	for (int j=0;j<N_H2;j++) {
		weights.b2[j] = b2[j];
	}
	for (int j=0;j<N_OUTPUT;j++) {
		weights.b3[j] = b3[j];
	}
	fwrite(&weights,sizeof(weights),1,writefp);
	fclose(writefp);
	return 0;
}
