#include <cstdio>
#include <cmath>
#include <cstdint>
#include <cuda.h>
#include "cuda_get_time/my_cuda_get_time.cuh"
#include <cuda_profiler_api.h>
#include "bitmap.hpp"
#include <fstream>

/*
#include <stdio.h>
#include <math.h>
#include<stdint.h>
#include<stdlib.h>
#include<cuda.h>
 */
#define WIDTH 1024
#define HEIGHT 1024
#define POINTS 284

__global__ void distance_gpu(int *x_d,int *y_d,float *z_d,float *img_buf_d)
{
   int i = blockDim.x*blockIdx.x+threadIdx.x;
   int j = blockDim.y*blockIdx.y+threadIdx.y;
   int k;
   float coef=49.63021f;
   for(k=0;k<284;k++){
      //for(k=0;k<POINTS;k++){
      img_buf_d[i*WIDTH+j]=img_buf_d[i*WIDTH+j]+cos(coef*float((j-x_d[k])*(j-x_d[k])+(i-y_d[k])*(i-y_d[k]))/z_d[k]);
   }
}

int main()
{

   FILE *datafp;
   int i,j;
   datafp=fopen("../../../data/cube284.3d","rb");
   if(datafp==NULL){
      printf("Can't open data file\n");
      return 1;
   }
   int x[POINTS];
   int y[POINTS];
   float z[POINTS];
   int x_buf,y_buf,z_buf;
   for(i=0;i<POINTS;i++){
      fread(&x_buf,sizeof(int),1,datafp);
      fread(&y_buf,sizeof(int),1,datafp);
      fread(&z_buf,sizeof(int),1,datafp);
      x[i]=x_buf*40+512;
      y[i]=y_buf*40+512;
      z[i]=((float)z_buf)*40+100000.0;
   }
   fclose(datafp);
   int *x_d,*y_d;
   float *z_d;
   float *img_buf_d;
   cudaMalloc((void**)&x_d,POINTS*sizeof(int));
   cudaMalloc((void**)&y_d,POINTS*sizeof(int));
   cudaMalloc((void**)&z_d,POINTS*sizeof(float));
   cudaMalloc((void**)&img_buf_d,WIDTH*HEIGHT*sizeof(float));
   float *img_buf;
   img_buf=(float *)malloc(sizeof(float)*WIDTH*HEIGHT);
   cudaMemcpy(x_d,x,POINTS*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(y_d,y,POINTS*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(z_d,z,POINTS*sizeof(float),cudaMemcpyHostToDevice);

   int thrX,thrY;
   float min,max,mid;
   thrX=10;
   cudaProfilerStart();
   cudatimeStamp cutime(101);
   for (thrY=1;thrY<50;thrY++) {
      for(i=0;i<WIDTH*HEIGHT;i++){
         img_buf[i]=0.0f;
      }
      cudaMemcpy(img_buf_d,img_buf,WIDTH*HEIGHT*sizeof(float),cudaMemcpyHostToDevice);
      dim3 threads(thrX,thrY,1);
      dim3 blocks(ceil(WIDTH/threads.x),ceil(HEIGHT/threads.y),1);
      cutime.stamp();
      distance_gpu<<<blocks,threads>>>(x_d,y_d,z_d,img_buf_d);
      cutime.stamp();
      cudaMemcpy(img_buf,img_buf_d,WIDTH*HEIGHT*sizeof(float),cudaMemcpyDeviceToHost);
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
      mid=0.5f*(min+max);
      printf("min = %lf  max = %lf  mid = %lf\n",min,max,mid);

      if (min != -70.310204f) {
         printf("min caliculation failure\n");
         cutime.print();
         return 1;
      }
      if (max != 75.139923f) {
         printf("max caliculation failure\n");
         cutime.print();
         return 1;
      }
   }

   std::ofstream ofs("timestamps.txt");
   for (i=0;i<cutime.index-1;i++)
      ofs<<cutime.interval(i)<<std::endl;
   cutime.print();

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
   Bitmap bmp(WIDTH,HEIGHT);
   FILE *bmpfp;
   bmpfp=fopen("cgh.bmp","wb");
   if(bmpfp==NULL){
      printf("Can't write file\n");
      return 1;
   }
   bmp.write(img,bmpfp);
   fclose(bmpfp);

   free(img);
   free(img_buf);
   cudaFree(x_d);
   cudaFree(y_d);
   cudaFree(z_d);
   cudaFree(img_buf_d);

   return 0;
}