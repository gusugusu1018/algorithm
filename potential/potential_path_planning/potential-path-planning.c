#include <stdio.h>
#include <stdlib.h>
#define NMAX 100
#define N_EPOCH 80
#define POINTS 1600

double U[NMAX][NMAX];

typedef struct {
	int x;
	int y;
} point_t;

void get_obstacle(int x[POINTS],int y[POINTS]);
point_t destination(point_t p);

void calc_potential(int ob_x[POINTS],int ob_y[POINTS],int p_x,int p_y,int g_x,int g_y,double U_d[NMAX][NMAX]) {
	int i,j,k,m;
	for(i=0;i<NMAX;i++) {
		for(j=0;j<NMAX;j++) {
			U_d[i][j]=0;
		}
	}
	for(k=0;k<=1000;k++) {
		for (m=0;m<POINTS;m++) U_d[ob_x[m]][ob_y[m]]=100.0;
		U_d[p_x][p_y]=100.0;
		U_d[g_x][g_y]=-100.0;
		for(i=1;i<(NMAX-1);i++) {
			for(j=1;j<(NMAX-1);j++) {
				U_d[i][j]=0.25*(U_d[i+1][j]+U_d[i-1][j]+U_d[i][j+1]+U_d[i][j-1]);
			}
		}
	}
}

int main()
{
	int i,j,k,n,m;
	int ob_x[POINTS];
	int ob_y[POINTS];
	point_t p = {20 ,20};
	point_t goal = {40 ,80};
	point_t dst;
	get_obstacle(ob_x,ob_y);
	FILE *potential;
	potential=fopen("potential.data","w");
	FILE *path;
	path=fopen("path.data","w");
	for (n=0;n<N_EPOCH;n++) {
		calc_potential(ob_x,ob_y,p.x,p.y,goal.x,goal.y,U);
		/*
		// input
		// ob_x,ob_y
		// p_x,p_y
		// g_x,g_y
		for(i=0;i<NMAX;i++) {
			for(j=0;j<NMAX;j++) {
				U[i][j]=0;
			}
		}
		for(k=0;k<=1000;k++) {
			for (m=0;m<POINTS;m++) U[ob_x[m]][ob_y[m]]=100.0;
			U[p.x][p.y]=100.0;
			U[goal.x][goal.y]=-100.0;
			for(i=1;i<(NMAX-1);i++) {
				for(j=1;j<(NMAX-1);j++) {
					U[i][j]=0.25*(U[i+1][j]+U[i-1][j]+U[i][j+1]+U[i][j-1]);
				}
			}
		}
		// output
		// U[NMAX][NMAX]
		*/
		dst=destination(p);
		if (U[dst.x][dst.y]>100) {
			return 1;
		}
		printf("%d %d %d\n",n,dst.x,dst.y);
		fprintf(path,"%d %d %d\n",dst.x,dst.y,100-n);
		if ((dst.x==p.x)&&(dst.y==p.y)) break;
		p.x=dst.x;
		p.y=dst.y;
	}
	for (m=0;m<POINTS;m++) U[ob_x[m]][ob_y[m]]=100.0;
	U[p.x][p.y]=100.0;
	U[goal.x][goal.y]=-100.0;
	for(i=0;i<NMAX;i++) {
		for(j=0;j<NMAX;j++) {
			fprintf(potential,"%d %d %f\n",i,j,U[i][j]);
		}
		fprintf(potential,"\n");
	}
	fclose(potential);
	fclose(path);
	return 0;
}

void get_obstacle(int x[POINTS],int y[POINTS]) {
	int i;
	FILE *readfp;
	if ((readfp = fopen("POINTS.2d", "rb")) == NULL) {
		printf("Can't open a file.");
		exit(1);
	}
	int x_buf;
	int y_buf;
	for (i=0;i<POINTS;i++) {
		fread(&x_buf,sizeof(int),1,readfp);
		fread(&y_buf,sizeof(int),1,readfp);
		x[i]=x_buf;
		y[i]=y_buf;
	}
	fclose(readfp);
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
}

