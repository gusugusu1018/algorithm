#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#define NMAX 100
#define N_EPOCH 160
#define POINTS 1600

double U[NMAX][NMAX];

void write_obstacle() {
	int i,j;
	FILE *writefp;
	if ((writefp=fopen("POINTS.2d","wb"))==NULL) {
		printf("Can't open a file");
		exit(1);
	}
	for(i=0;i<60;i++) {
		for(j=40;j<60;j++) {
			fwrite(&i,sizeof(int),1,writefp);
			fwrite(&j,sizeof(int),1,writefp);
		}
	}
	j=0;
	for (i=0;i<NMAX;i++) {
		fwrite(&i,sizeof(int),1,writefp);
		fwrite(&j,sizeof(int),1,writefp);
	}
	for (i=0;i<NMAX;i++) {
		fwrite(&j,sizeof(int),1,writefp);
		fwrite(&i,sizeof(int),1,writefp);
	}
	j=NMAX-1;
	for (i=0;i<NMAX;i++) {
		fwrite(&i,sizeof(int),1,writefp);
		fwrite(&j,sizeof(int),1,writefp);
	}
	for (i=0;i<NMAX;i++) {
		fwrite(&j,sizeof(int),1,writefp);
		fwrite(&i,sizeof(int),1,writefp);
	}
	fclose(writefp);
}

void get_obstacle(int x[POINTS],int y[POINTS]) {
	int i=0;
	FILE *readfp;
	if ((readfp = fopen("POINTS.2d", "rb")) == NULL) {
		printf("Can't open a file.");
		exit(1);
	}
	int x_buf;
	int y_buf;
	for (int i=0;i<POINTS;i++) {
		fread(&x_buf,sizeof(int),1,readfp);
		fread(&y_buf,sizeof(int),1,readfp);
		x[i]=x_buf;
		y[i]=y_buf;
	}
	fclose(readfp);
}

int main()
{
	int x[POINTS];
	int y[POINTS];
	printf("write obstacle points\n");
	write_obstacle();
	printf("get obstacle points\n");
	get_obstacle(x,y);
	printf("print obstacle points\n");
	for (int i=0;i<POINTS;i++) {
		printf ("%d,%d\n", x[i],y[i]);
	}
	return 0;
}

