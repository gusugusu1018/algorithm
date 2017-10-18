#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdint>
#include <sys/time.h>
#include <sys/resource.h>
#include <cuda.h>
#include "bitmap.hpp"

__global__ void distance_gpu(int *x_d,int *y_d,float *z_d,float *img_buf_d,int *points_d,int _width)
{
   int i,j,k;

   i=blockIdx.x;
   j=threadIdx.x;

   float kankaku,hatyou,goukei,pi;

   hatyou=0.633F;
   kankaku=10.5F;
   pi=3.14159265F;
   goukei=2.0F*pi*kankaku/hatyou;

   float dx,dy,tmp;

   tmp=0.0F;

   for(k=0;k<*points_d;k++){
      dx=(float)(x_d[k]-j);
      dy=(float)(y_d[k]-i);
      tmp=tmp+cos(goukei*0.5F*(dx*dx+dy*dy)/z_d[k]);
   }
   img_buf_d[i*_width+j] = tmp;
}

int main(){
   const static int width = 1024;
   const static int height = 1024;
   int points;
   FILE *fp;
   int i,j;
   Bitmap bmp(width, height);

   fp=fopen("../../../data/ten1.3d","rb");
   if(fp==NULL){
      printf("Can't open file\n");
   }

   fread(&points,sizeof(int),1,fp);

   int x[points];
   int y[points];
   float z[points];
   int *points_d;
   cudaMemcpy(points_d,&points,sizeof(int),cudaMemcpyHostToDevice);

   int *x_d,*y_d;
   float *z_d;
   float *img_buf_d;

   dim3 blocks(1024,1,1);
   dim3 threads(1024,1,1);

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

   cudaMalloc((void**)&x_d,points*sizeof(int));
   cudaMalloc((void**)&y_d,points*sizeof(int));
   cudaMalloc((void**)&z_d,points*sizeof(float));

   cudaMalloc((void**)&img_buf_d,width*height*sizeof(float));

   float *img_buf;

   img_buf=(float *)malloc(sizeof(float)*width*height);
   for(i=0;i<width*height;i++){
      img_buf[i]=0.0f;
   }

   cudaMemcpy(x_d,x,points*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(y_d,y,points*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(z_d,z,points*sizeof(float),cudaMemcpyHostToDevice);

   cudaMemcpy(img_buf_d,img_buf,width*height*sizeof(float),cudaMemcpyHostToDevice);

   distance_gpu<<<blocks,threads>>>(x_d,y_d,z_d,img_buf_d,points_d,width);

   cudaMemcpy(img_buf,img_buf_d,width*height*sizeof(float),cudaMemcpyDeviceToHost);

   float min,max,mid;

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

   free(img);
   free(img_buf);
   fclose(bmpFp);
   cudaFree(points_d);
   cudaFree(x_d);
   cudaFree(y_d);
   cudaFree(z_d);
   cudaFree(img_buf_d);

   return 0;
}
