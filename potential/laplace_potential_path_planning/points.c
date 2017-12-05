#include <stdio.h>
#define NMAX 100
#define N_EPOCH 160

typedef struct {
	int x;
	int y;
} point_t;

double U[NMAX][NMAX];

void obstacle() {
	int i,j;
	for(i=0;i<60;i++) {
		for(j=40;j<60;j++) {
			U[i][j]=100;
		}
	}
	for (i=0;i<NMAX;i++) U[0][i]=100;
	for (i=0;i<NMAX;i++) U[NMAX-1][i]=100;
	for (i=0;i<NMAX;i++) U[i][NMAX-1]=100;
	for (i=0;i<NMAX;i++) U[i][0]=100;
}

point_t destination(point_t p) {
	point_t min;
	min.x=p.x;
	min.y=p.y;
	if (U[p.x][p.y-1]<U[min.x][min.y]) {
		min.x=p.x;
		min.y=p.y-1;
	}
	if (U[p.x][p.y+1]<U[min.x][min.y]) {
		min.x=p.x;
		min.y=p.y+1;
	}
	if (U[p.x-1][p.y]<U[min.x][min.y]) {
		min.x=p.x-1;
		min.y=p.y;
	}
	if (U[p.x+1][p.y]<U[min.x][min.y]) {
		min.x=p.x+1;
		min.y=p.y;
	}
	if (U[p.x+1][p.y+1]<U[min.x][min.y]) {
		min.x=p.x+1;
		min.y=p.y+1;
	}
	if (U[p.x+1][p.y-1]<U[min.x][min.y]) {
		min.x=p.x+1;
		min.y=p.y-1;
	}
	if (U[p.x-1][p.y+1]<U[min.x][min.y]) {
		min.x=p.x-1;
		min.y=p.y+1;
	}
	if (U[p.x-1][p.y-1]<U[min.x][min.y]) {
		min.x=p.x-1;
		min.y=p.y-1;
	}
	if (U[p.x][p.y-2]<U[min.x][min.y]) {
		min.x=p.x;
		min.y=p.y-2;
	}
	if (U[p.x][p.y+2]<U[min.x][min.y]) {
		min.x=p.x;
		min.y=p.y+2;
	}
	if (U[p.x-2][p.y]<U[min.x][min.y]) {
		min.x=p.x-2;
		min.y=p.y;
	}
	if (U[p.x+2][p.y]<U[min.x][min.y]) {
		min.x=p.x+2;
		min.y=p.y;
	}

	return min;
	//pathx[n]=min.x;
	//pathy[n]=min.y;
}

int main()
{
	int i,j,k,n;
	point_t p = {20 ,20};
	point_t goal = {40 ,80};
	point_t dst;
	FILE *output;
	output=fopen("output.data","w");
	for (n=0;n<N_EPOCH;n++) {
		for(i=0;i<NMAX;i++) {
			for(j=0;j<NMAX;j++) {
				U[i][j]=0;
			}
		}
		obstacle();
		U[p.x][p.y]=100.0;
		U[goal.x][goal.y]=-100.0;
		for(k=0;k<=1000;k++) {
			for(i=1;i<(NMAX-1);i++) {
				for(j=1;j<(NMAX-1);j++) {
					U[i][j]=0.25*(U[i+1][j]+U[i-1][j]+U[i][j+1]+U[i][j-1]);
				}
			}
			obstacle();
			U[p.x][p.y]=100.0;
			U[goal.x][goal.y]=-100.0;
		}
		dst=destination(p);
		if (U[dst.x][dst.y]>100) {
			return 1;
		}
		//printf("%f,%d,%d\n",U[min.x][min.y],min.x,min.y);
		//printf("%d %d %d\n",n,dst.x,dst.y);
		printf("%d %d %d\n",dst.x,dst.y,100-n);
		if ((dst.x==p.x)&&(dst.y==p.y)) break;
		p.x=dst.x;
		p.y=dst.y;
	}

	//printf("finish\n");
	for(i=0;i<NMAX;i++) {
		for(j=0;j<NMAX;j++) {
			fprintf(output,"%d %d %f\n",i,j,U[i][j]);
		}
		fprintf(output,"\n");
	}
	fclose(output);

	return 0;
}
