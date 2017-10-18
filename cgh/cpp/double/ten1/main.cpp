#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdint>
#include <sys/time.h>
#include <sys/resource.h>
#include "bitmap.hpp"

double getrusage_sec()
{
	struct rusage t;
	struct timeval tv;

	getrusage(RUSAGE_SELF,&t);
	tv = t.ru_utime;

	return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

int main(){
   const static int width = 1920;
   const static int height = 1080;
	double startTime,endTime;
	int points;
	FILE *fp;
	int i,j,k;
   Bitmap bmp(width, height);

	fp=fopen("../../../data/ten1.3d","rb");
	if(fp==NULL){
		printf("Can't open file\n");
	}

	fread(&points,sizeof(int),1,fp);

	int x[points];
	int y[points];
	double z[points];

	int x_buf,y_buf,z_buf;

	for(i=0;i<points;i++){
		fread(&x_buf,sizeof(int),1,fp);
		fread(&y_buf,sizeof(int),1,fp);
		fread(&z_buf,sizeof(int),1,fp);

		x[i]=x_buf*40+960;
		y[i]=y_buf*40+540;
		z[i]=((double)z_buf)*40+100000.0;
	}
	fclose(fp);

	double *img_buf;

	img_buf=(double *)malloc(sizeof(double)*width*height);
	for(i=0;i<width*height;i++){
		img_buf[i]=0.0;
	}


	double intarval,lambda,sum;

	lambda=0.633;
	intarval=10.5;
	sum=2.0*M_PI*intarval/lambda;

	startTime = getrusage_sec();
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			for(k=0;k<points;k++){
				//img_buf[i*width+j]=img_buf[i*width+j]+cos(sum*distance_horo_objects(x[k],y[k],z[k],j,i));
          			img_buf[i*width+j]=img_buf[i*width+j]+cos(sum*sqrt((j-x[k])*(j-x[k])+(i-y[k])*(i-y[k])+z[k]*z[k]));
			}
		}
	}
	endTime = getrusage_sec();

	double min,max,mid;

	min=img_buf[0];
	max=img_buf[0];

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(min>img_buf[i*width+j]){
				min=img_buf[i*width+j];
			}
			if(max<img_buf[i*width+j]){
				max=img_buf[i*width+j];
			}
		}
	}

	mid=0.5*(min+max);

	printf("min = %lf  max = %lf  mid = %lf\n",min,max,mid);


	unsigned char *img;
	img=(unsigned char *)malloc(sizeof(unsigned char)*width*height);

	for(i=0;i<width*height;i++){
		if(img_buf[i]<mid){
			img[i]=0;
		}
		if(img_buf[i]>mid){
			img[i]=255;
		}
	}

	FILE *bmpFp;
	bmpFp=fopen("cgh.bmp","wb");
	if(bmpFp==NULL){
		printf("Can't open file\n");
	}
   bmp.write(img,bmpFp);

	printf("Calculation time is %lf\n",endTime-startTime);

	free(img);
	free(img_buf);
	fclose(bmpFp);

	return 0;
}
