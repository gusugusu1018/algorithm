#include <stdio.h>
#include <math.h>
#include<stdint.h>
#include<stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#define WIDTH 1920
#define HEIGHT 1080

#pragma pack(push,1)
typedef struct tagBITMAPFILEHEADER
{
	unsigned short bfType;
	uint32_t  bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	uint32_t  bf0ffBits;
}BITMAPFILEHEADER;
#pragma pack(pop)

typedef struct tagBITMAPINFOHEADER
{
	uint32_t  biSize;
	int32_t	biWidth;
	int32_t	biHeight;
	unsigned short  biPlanes;
	unsigned short  biBitCount;
	uint32_t   biCompression;
	uint32_t   biSizeImage;
	int32_t	 biXPelsPerMeter;
	int32_t	 biYPelsPerMeter;
	uint32_t   biCirUsed;
	uint32_t   biCirImportant;
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
	unsigned char  rgbBlue;
	unsigned char  rgbGreen;
	unsigned char  rgbRed;
	unsigned char  rgbReserved;
}RGBQUAD;

typedef struct tagBITMAPINFO
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD          bmiColors[1];
}BITMAPINFO;


double getrusage_sec()
{
	struct rusage t;
	struct timeval tv;

	getrusage(RUSAGE_SELF,&t);
	tv = t.ru_utime;

	return tv.tv_sec + (double)tv.tv_usec*1e-6;
}


float distance_horo_objects_easy(int xj,int yj,float zj,int xa,int ya){
	float r_ja;
	int dx;
	int dy;

	dx=xa-xj;
	dy=ya-yj;

	r_ja=(dx*dx+dy*dy)/zj;

	return r_ja;
}

int main(){

	double starttime1,endtime1;

	int points;

	BITMAPFILEHEADER    BmpFileHeader;
	BITMAPINFOHEADER    BmpInfoHeader;
	RGBQUAD             RGBQuad[256];

	FILE *fp;
	int i,j,k;

	BmpFileHeader.bfType                =19778;
	BmpFileHeader.bfSize                =14+40+1024+(WIDTH*HEIGHT);
	BmpFileHeader.bfReserved1           =0;
	BmpFileHeader.bfReserved2           =0;
	BmpFileHeader.bf0ffBits             =14+40+1024;

	BmpInfoHeader.biSize                =40;
	BmpInfoHeader.biWidth               =WIDTH;
	BmpInfoHeader.biHeight              =HEIGHT;
	BmpInfoHeader.biPlanes              =1;
	BmpInfoHeader.biBitCount            =8;     //256階調
	BmpInfoHeader.biCompression         =0L;
	BmpInfoHeader.biSizeImage           =0L;
	BmpInfoHeader.biXPelsPerMeter       =0L;
	BmpInfoHeader.biYPelsPerMeter       =0L;
	BmpInfoHeader.biCirUsed             =0L;
	BmpInfoHeader.biCirImportant        =0L;

	for(i=0;i<256;i++){
		RGBQuad[i].rgbBlue                =i;
		RGBQuad[i].rgbGreen               =i;
		RGBQuad[i].rgbRed                 =i;
		RGBQuad[i].rgbReserved            =0;
	}

	fp=fopen("../../../data/cube284.3d","rb");
	if(fp==NULL){
		printf("Can't open file\n");
	}

	fread(&points,sizeof(int),1,fp);

	int x[points];
	int y[points];
	float z[points];

	int x_buf,y_buf,z_buf;

	for(i=0;i<points;i++){
		fread(&x_buf,sizeof(int),1,fp);
		fread(&y_buf,sizeof(int),1,fp);
		fread(&z_buf,sizeof(int),1,fp);

		x[i]=x_buf*40+960;
		y[i]=y_buf*40+540;
		z[i]=((float)z_buf)*40+100000.0;
	}
	fclose(fp);

	/*
	   for(i=0;i<points;i++){
	   printf("%d   %d   %d   %f\n",i,x[i],y[i],z[i]);
	   }
	 */

	float *img_buf;

	img_buf=(float *)malloc(sizeof(float)*WIDTH*HEIGHT);
	for(i=0;i<WIDTH*HEIGHT;i++){
		img_buf[i]=0.0;
	}


	float lambda,dtp,h,dx,dy;

	lambda=0.0633f;
	dtp=10.5f;
	h=M_PI*dtp/lambda;

	starttime1 = getrusage_sec();
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			for(k=0;k<points;k++){
				//img_buf[i*WIDTH+j]=img_buf[i*WIDTH+j]+cos(h*distance_horo_objects_easy(x[k],y[k],z[k],j,i));
				dx=(float)j-x[k];
				dy=(float)i-y[k];
				img_buf[i*WIDTH+j]=img_buf[i*WIDTH+j]+cos(h*(dx*dx+dy*dy)/z[k]);
				//img_buf[i*WIDTH+j]=img_buf[i*WIDTH+j]+cos(h*((j-x[k])*(j-x[k])+(i-y[k])*(i-y[k]))/z[k]);
			}
		}
	}
	endtime1 = getrusage_sec();

	float min,max,mid;

	min=img_buf[0];
	max=img_buf[0];

	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			if(min>img_buf[i*WIDTH+j]){
				min=img_buf[i*WIDTH+j];
			}
			if(max<img_buf[i*WIDTH+j]){
				max=img_buf[i*WIDTH+j];
			}
		}
	}

	mid=0.5*(min+max);

	printf("min = %lf  max = %lf  mid = %lf\n",min,max,mid);


	unsigned char *img;
	img=(unsigned char *)malloc(sizeof(unsigned char)*WIDTH*HEIGHT);

	for(i=0;i<WIDTH*HEIGHT;i++){
		if(img_buf[i]<mid){
			img[i]=0;
		}
		if(img_buf[i]>mid){
			img[i]=255;
		}
	}

	FILE *fp1;
	fp1=fopen("cgh.bmp","wb");
	if(fp1==NULL){
		printf("Can't open file\n");
	}

	fwrite(&BmpFileHeader, sizeof(BmpFileHeader) , 1 ,fp1);
	fwrite(&BmpInfoHeader, sizeof(BmpInfoHeader) , 1 ,fp1);
	fwrite(&RGBQuad[0], sizeof(RGBQuad[0]) , 256 ,fp1);
	fwrite(img,sizeof(unsigned char),WIDTH*HEIGHT,fp1);

	printf("Calculation time is %lf\n",endtime1-starttime1);

	free(img);
	free(img_buf);
	fclose(fp1);

	return 0;
}
