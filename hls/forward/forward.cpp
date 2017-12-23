#include "network.hpp"

data_t relu_fixed(data_t a) {
	if (a > 0.0) return a;
	else return 0.0;
}

void forward_fixed(const unsigned char src_data[N_INPUT],
                data_t dst_data[N_OUTPUT],
                const data_t w01[N_INPUT*N_H1],
                const data_t b1[N_H1],
				const data_t w12[N_H1*N_H2],
				const data_t b2[N_H2],
				const data_t w23[N_H2*N_OUTPUT],
				const data_t b3[N_OUTPUT])
{
/*
	data_t data[N_INPUT];
	// standlize
	for (int j=0;j<28*28;j++) {
#pragma HLS unroll factor 784
		data[j] = src_data[j]/255.0f;
	}
*/
	data_t z1[N_H1];
	data_t a1[N_H1];
	// H1 perseptron 1024kai
	for (int j=0;j<N_H1;j++) {
//#pragma HLS unroll factor 2
//#pragma HLS pipeline II=1
		// perseptron 784->1
		z1[j] = 0.0f;
		for (int i=0;i<N_INPUT;i++) {
			// sum w*data
			//z1[j]+=w01[i][j]*data[i];
			//z1[j]+=w01[i*N_H1+j]*data[i];
			z1[j]+=w01[i*N_H1+j]*(data_t)(src_data[i]/255.0);
		}
		// sum bias
		z1[j]+=b1[j];
		// activation function
		// relu
		a1[j]=relu_fixed(z1[j]);
	}

	data_t z2[N_H2];
	data_t a2[N_H2];
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
		a2[j]=relu_fixed(z2[j]);
	}
	data_t z3[N_OUTPUT];
	data_t a3[N_OUTPUT];
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
	data_t max = 0.0f;
	data_t sum = 0.0f;
	// overflow avoidance
	LOOP_SOFTMAX_1:for (int i=0; i<N_OUTPUT; i++) if(max < z3[i]) max = z3[i];
	LOOP_SOFTMAX_2:for (int i=0; i<N_OUTPUT; i++) {
		data_t temp = z3[i]-max;
		a3[i] = hls::exp(temp.to_float());
		//a3[i] = hls::exp(z3[i] - max);
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
}
/*
float relu(float a) {
	return a > 0.0f ? a:0.0f;
}

void forward(const unsigned char src_data[N_INPUT],
                float dst_data[N_OUTPUT],
                float w01[N_INPUT*N_H1],
                float b1[N_H1],
                float w12[N_H1*N_H2],
                float b2[N_H2],
                float w23[N_H2*N_OUTPUT],
                float b3[N_OUTPUT])
{
*/
/*
	float data[N_INPUT];
	// standlize
	for (int j=0;j<28*28;j++) {
#pragma HLS unroll factor 784
		data[j] = src_data[j]/255.0f;
	}
*/
/*
	float z1[N_H1];
	float a1[N_H1];
	// H1 perseptron 1024kai
	for (int j=0;j<N_H1;j++) {
//#pragma HLS unroll factor 2
//#pragma HLS pipeline II=1
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
}
*/
