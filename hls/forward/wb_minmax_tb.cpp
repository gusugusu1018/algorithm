#include "network.hpp"
#define MAX_FILENAME 30
#define N_TRAIN_DATA 60000
#include "mnist.h"
#include "weights.h"

float data[N_INPUT];
float w01[N_INPUT*N_H1];
float b1[N_H1];
float w12[N_H1*N_H2];
float b2[N_H2];
float w23[N_H2*N_OUTPUT];
float b3[N_OUTPUT];

int main(){

   char save_file[MAX_FILENAME]={"new.weights"};
   std::cout<<"init dataset..."<<std::endl;
   std::vector<std::vector<float> > train_data;
   std::vector<float> label_data;
   Mnist mnist;
   train_data = mnist.readTrainingFile("train-images-idx3-ubyte");
   label_data = mnist.readLabelFile("train-labels-idx1-ubyte");

   char load_file[MAX_FILENAME]={"new.weights"};
   int i,j,k;
   //****************get weight and bias*******************
   std::cout<<"get weights"<<std::endl;
   get_weights(load_file,w01,b1,w12,b2,w23,b3);
   //*************************forward****************************
   //for (int i=0;i<N_INPUT*N_H1;i++)std::cout<<w01[i]<<std::endl;
   //for (int i=0;i<N_H1;i++)std::cout<<b1[i]<<std::endl;
   float max=0.0f;
   float min=0.0f;
   for (int i=0;i<N_INPUT*N_H1;i++) {
	   if (max<w01[i])max=w01[i];
	   if (min>w01[i])min=w01[i];
   }
   std::cout<<"w01"<<std::endl;
   std::cout<<max<<std::endl;
   std::cout<<min<<std::endl;
   max=0.0f;
   min=0.0f;
   for (int i=0;i<N_H1;i++) {
	   if (max<b1[i])max=b1[i];
	   if (min>b1[i])min=b1[i];
   }
   std::cout<<"b1"<<std::endl;
   std::cout<<max<<std::endl;
   std::cout<<min<<std::endl;
   max=0.0f;
   min=0.0f;
   for (int i=0;i<N_H1*N_H2;i++) {
	   if (max<w12[i])max=w12[i];
	   if (min>w12[i])min=w12[i];
   }
   std::cout<<"w12"<<std::endl;
   std::cout<<max<<std::endl;
   std::cout<<min<<std::endl;
   max=0.0f;
   min=0.0f;
   for (int i=0;i<N_H2;i++) {
	   if (max<b2[i])max=b2[i];
	   if (min>b2[i])min=b2[i];
   }
   std::cout<<"b2"<<std::endl;
   std::cout<<max<<std::endl;
   std::cout<<min<<std::endl;
   max=0.0f;
   min=0.0f;
   for (int i=0;i<N_H2*N_OUTPUT;i++) {
	   if (max<w23[i])max=w23[i];
	   if (min>w23[i])min=w23[i];
   }
   std::cout<<"w23"<<std::endl;
   std::cout<<max<<std::endl;
   std::cout<<min<<std::endl;
   max=0.0f;
   min=0.0f;
   for (int i=0;i<N_OUTPUT;i++) {
	   if (max<b3[i])max=b3[i];
	   if (min>b3[i])min=b3[i];
   }
   std::cout<<"b3"<<std::endl;
   std::cout<<max<<std::endl;
   std::cout<<min<<std::endl;

/*
   // input data
   unsigned char src_data[28*28];
   for (int j=0;j<28*28;j++) {
	   src_data[j] = (unsigned char)train_data[0][j];
   }
   float dst_data[10];
   forward(src_data,dst_data,w01,b1,w12,b2,w23,b3);
   for (i=0;i<10;i++)std::cout<<i<<" "<<dst_data[i]<<std::endl;
   std::cout<<"the teacher is"<<label_data[0]<<std::endl;
*/
   return 0;
}
