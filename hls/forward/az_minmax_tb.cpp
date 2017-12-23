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

   // input data
   unsigned char src_data[28*28];
   for (int j=0;j<28*28;j++) {
	   src_data[j] = (unsigned char)train_data[0][j];
   }
   float dst_data[10];
   forward_cpu(src_data,dst_data,w01,b1,w12,b2,w23,b3);
   for (i=0;i<10;i++)std::cout<<i<<" "<<dst_data[i]<<std::endl;
   std::cout<<"the teacher is"<<label_data[0]<<std::endl;

   return 0;
}


float relu_cpu(float a) {
	return a > 0.0f ? a:0.0f;
}

void forward_cpu(const unsigned char src_data[N_INPUT],
                float dst_data[N_OUTPUT],
                float w01[N_INPUT*N_H1],
                float b1[N_H1],
                float w12[N_H1*N_H2],
                float b2[N_H2],
                float w23[N_H2*N_OUTPUT],
                float b3[N_OUTPUT])
{
/*
	float data[N_INPUT];
	// standlize
	for (int j=0;j<28*28;j++) {
		data[j] = src_data[j]/255.0f;
	}
*/
	float z1[N_H1];
	float a1[N_H1];
	// H1 perseptron 1024kai
	for (int j=0;j<N_H1;j++) {
		// perseptron 784->1
		z1[j] = 0.0f;
		for (int i=0;i<N_INPUT;i++) {
			// sum w*data
			//z1[j]+=w01[i][j]*data[i];
			//z1[j]+=w01[i*N_H1+j]*data[i];
			z1[j]+=w01[i*N_H1+j]*src_data[i]/255.0f;
		}
		// sum bias
		z1[j]+=b1[j];
		// activation function
		// relu
		a1[j]=relu(z1[j]);
	}

	float z2[N_H2];
	float a2[N_H2];
	// H2 perseptron 1024kai
	for (int j=0;j<N_H2;j++) {
		// perseptron 1024->1
		z2[j] = 0.0f;
		for (int i=0;i<N_H1;i++) {
			// sum w*a1
			//z2[j]+=w12[i][j]*a1[i];
			z2[j]+=w12[i*N_H2+j]*a1[i];
		}
		// sum bias
		z2[j]+=b2[j];
		// activation function
		// relu
		a2[j]=relu(z2[j]);
	}
	float z3[N_OUTPUT];
	float a3[N_OUTPUT];
	// OUTPUT layer
	for (int j=0;j<N_OUTPUT;j++) {
		// perseptron 1024->1
		z3[j] = 0.0f;
		for (int i=0;i<N_H2;i++) {
			// sum w*a1
			//z3[j]+=w23[i][j]*a2[i];
			z3[j]+=w23[i*N_OUTPUT+j]*a2[i];
		}
		// sum bias
		z3[j]+=b3[j];
	}

	// activation function
	// softmax
	float max = 0.0f;
	float sum = 0.0f;
	// overflow avoidance
	LOOP_SOFTMAX_1:for (int i=0; i<N_OUTPUT; i++) if(max < z3[i]) max = z3[i];
	LOOP_SOFTMAX_2:for (int i=0; i<N_OUTPUT; i++) {
		a3[i] = hls::exp(z3[i] - max);
		//a3[i] = exp(z3[i] - max);
		//sum += a3[i];
	}
	sum = a3[0]+a3[1]+a3[2]+a3[3]+a3[4]+a3[5]+a3[6]+a3[7]+a3[8]+a3[9];
	LOOP_SOFTMAX_3:for (int i=0; i<N_OUTPUT; i++){
		 a3[i] /= sum;
	}
	LOOP_OUTPUT:for (int i=0; i<N_OUTPUT; i++) {
		dst_data[i] = a3[i];
	}
	   max=0.0f;
	   float min=0.0f;
	   for (int i=0;i<N_H1;i++) {
		   if (max<z1[i])max=z1[i];
		   if (min>z1[i])min=z1[i];
	   }
	   std::cout<<"z1"<<std::endl;
	   std::cout<<max<<std::endl;
	   std::cout<<min<<std::endl;
	   max=0.0f;
	   min=0.0f;
	   for (int i=0;i<N_H1;i++) {
		   if (max<a1[i])max=a1[i];
		   if (min>a1[i])min=a1[i];
	   }
	   std::cout<<"a1"<<std::endl;
	   std::cout<<max<<std::endl;
	   std::cout<<min<<std::endl;
	   max=0.0f;
	   min=0.0f;
	   for (int i=0;i<N_H2;i++) {
		   if (max<z2[i])max=z2[i];
		   if (min>z2[i])min=z2[i];
	   }
	   std::cout<<"z2"<<std::endl;
	   std::cout<<max<<std::endl;
	   std::cout<<min<<std::endl;
	   max=0.0f;
	   min=0.0f;
	   for (int i=0;i<N_H2;i++) {
		   if (max<a2[i])max=a2[i];
		   if (min>a2[i])min=a2[i];
	   }
	   std::cout<<"a2"<<std::endl;
	   std::cout<<max<<std::endl;
	   std::cout<<min<<std::endl;
	   max=0.0f;
	   min=0.0f;
	   for (int i=0;i<N_OUTPUT;i++) {
		   if (max<z3[i])max=z3[i];
		   if (min>z3[i])min=z3[i];
	   }
	   std::cout<<"z3"<<std::endl;
	   std::cout<<max<<std::endl;
	   std::cout<<min<<std::endl;
	   max=0.0f;
	   min=0.0f;
	   for (int i=0;i<N_OUTPUT;i++) {
		   if (max<a3[i])max=a3[i];
		   if (min>a3[i])min=a3[i];
	   }
	   std::cout<<"a3"<<std::endl;
	   std::cout<<max<<std::endl;
	   std::cout<<min<<std::endl;
}
