#include <stdio.h>
#include <math.h>
#include<stdint.h>
#include<stdlib.h>
#include<cuda.h>


#define WID 1024
#define HEI 1024

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


__global__ void distance_gpu(int *x_d,int *y_d,double *z_d,double *img_buf_d,int *tensuu_d)
{
  int i,j,k;

  i=blockIdx.x;
  j=threadIdx.x;

  double hatyou,goukei;

  hatyou=0.0633;
  goukei=M_PI/hatyou;


  for(k=0;k<*tensuu_d;k++){
    img_buf_d[i*WID+j]=img_buf_d[i*WID+j]+cos(goukei*((j-x_d[k])*(j-x_d[k])+(i-y_d[k])*(i-y_d[k]))/z_d[k]);
  }
  
}


int main(){

    int tensuu;

    BITMAPFILEHEADER    BmpFileHeader;
    BITMAPINFOHEADER    BmpInfoHeader;
    RGBQUAD             RGBQuad[256];

    FILE *fp;
    int i,j;

    BmpFileHeader.bfType                =19778;
    BmpFileHeader.bfSize                =14+40+1024+(WID*HEI);
    BmpFileHeader.bfReserved1           =0;
    BmpFileHeader.bfReserved2           =0;
    BmpFileHeader.bf0ffBits             =14+40+1024;

    BmpInfoHeader.biSize                =40;
    BmpInfoHeader.biWidth               =WID;
    BmpInfoHeader.biHeight              =HEI;
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

    char filename[20]={};
    printf("ファイル名を入力してください :  ");
    scanf("%s",filename);

    fp=fopen(filename,"rb");
    if(fp==NULL){
      printf("ファイルオープンエラー\n");
    }

    fread(&tensuu,sizeof(int),1,fp);
    printf("物体点数は%dです\n",tensuu);

    int x[tensuu];
    int y[tensuu];
    double z[tensuu];

    int *tensuu_d;

    cudaMalloc((void**)&tensuu_d,sizeof(int));
    cudaMemcpy(tensuu_d,&tensuu,sizeof(int),cudaMemcpyHostToDevice);

    int *x_d,*y_d;
    double *z_d;
    double *img_buf_d;

    dim3 blocks(1024,1,1);
    dim3 threads(1024,1,1);

    int x_buf,y_buf,z_buf;

    for(i=0;i<tensuu;i++){
      fread(&x_buf,sizeof(int),1,fp);
      fread(&y_buf,sizeof(int),1,fp);
      fread(&z_buf,sizeof(int),1,fp);

      x[i]=x_buf*40+512;
      y[i]=y_buf*40+512;
      z[i]=((double)z_buf)*40+100000.0;
    }
    fclose(fp);


    cudaMalloc((void**)&x_d,tensuu*sizeof(int));
    cudaMalloc((void**)&y_d,tensuu*sizeof(int));
    cudaMalloc((void**)&z_d,tensuu*sizeof(double));

    cudaMalloc((void**)&img_buf_d,WID*HEI*sizeof(double));

    double *img_buf;

    img_buf=(double *)malloc(sizeof(double)*WID*HEI);
    for(i=0;i<WID*HEI;i++){
      img_buf[i]=0.0;
    }

    cudaMemcpy(x_d,x,tensuu*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(y_d,y,tensuu*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(z_d,z,tensuu*sizeof(double),cudaMemcpyHostToDevice);

    cudaMemcpy(img_buf_d,img_buf,WID*HEI*sizeof(double),cudaMemcpyHostToDevice);

    distance_gpu<<<blocks,threads>>>(x_d,y_d,z_d,img_buf_d,tensuu_d);

    cudaMemcpy(img_buf,img_buf_d,WID*HEI*sizeof(double),cudaMemcpyDeviceToHost);




    double min,max,mid;

    min=img_buf[0];
    max=img_buf[0];

    for(i=0;i<HEI;i++){
      for(j=0;j<WID;j++){
        if(min>img_buf[i*WID+j]){
          min=img_buf[i*WID+j];
        }
        if(max<img_buf[i*WID+j]){
          max=img_buf[i*WID+j];
        }
      }
    }

    mid=0.5*(min+max);

    printf("min = %lf  max = %lf  mid = %lf\n",min,max,mid);


    unsigned char *img;
    img=(unsigned char *)malloc(sizeof(unsigned char)*WID*HEI);

    for(i=0;i<WID*HEI;i++){
      if(img_buf[i]<mid){
        img[i]=0;
      }
      if(img_buf[i]>mid){
        img[i]=255;
      }
    }

    FILE *fp1;
    fp1=fopen("cgh_fresnel_gpu.bmp","wb");
    if(fp1==NULL){
      printf("ファイルオープンエラー\n");
    }

    fwrite(&BmpFileHeader, sizeof(BmpFileHeader) , 1 ,fp1);
    fwrite(&BmpInfoHeader, sizeof(BmpInfoHeader) , 1 ,fp1);
    fwrite(&RGBQuad[0], sizeof(RGBQuad[0]) , 256 ,fp1);
    fwrite(img,sizeof(unsigned char),WID*HEI,fp1);

    free(img);
    free(img_buf);
    fclose(fp1);

    cudaFree(tensuu_d);
    cudaFree(x_d);
    cudaFree(y_d);
    cudaFree(z_d);
    cudaFree(img_buf_d);


    return 0;



}
