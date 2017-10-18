#include <boost/thread.hpp>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "bitmap.h"

#define WIDTH 1920
#define HEIGHT 1080

double getrusage_sec(void );
void cgh(void );

void sl() {
   int counter=1;
   std::string command_str("figlet ");
   std::string num_str = std::to_string(counter);
   while(1) {
      system(("figlet " + num_str + " SL").c_str());
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      system("sl");
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      counter++;
      num_str = std::to_string(counter);
   }
}

int main(int argc, char const* argv[])
{
   boost::thread th1(cgh);
   boost::thread th2(sl);
   th1.join();
   th2.interrupt();
   sleep(6); 
   return 0;
}

double getrusage_sec()
{
   struct rusage t;
   struct timeval tv;

   getrusage(RUSAGE_SELF,&t);
   tv = t.ru_utime;

   return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

void cgh(void ) {
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

   fp=fopen("../data/cube284.3d","rb");
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

   img_buf=(double *)malloc(sizeof(double)*WIDTH*HEIGHT);
   for(i=0;i<WIDTH*HEIGHT;i++){
      img_buf[i]=0.0;
   }

   double intarval,lambda,sum;

   lambda=0.633;
   intarval=10.5;
   sum=2.0*M_PI*intarval/lambda;

   starttime1 = getrusage_sec();
   for(i=0;i<HEIGHT;i++){
      for(j=0;j<WIDTH;j++){
         for(k=0;k<points;k++){
            img_buf[i*WIDTH+j]=img_buf[i*WIDTH+j]+cos(sum*sqrt((j-x[k])*(j-x[k])+(i-y[k])*(i-y[k])+z[k]*z[k]));
         }
      }
   }
   endtime1 = getrusage_sec();

   double min,max,mid;

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
}
