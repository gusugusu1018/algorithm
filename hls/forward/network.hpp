#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "hls_math.h"

#define N_INPUT 784
#define N_H1    1024
#define N_H2    1024
#define N_OUTPUT        10

//typedef float data_t;
typedef ap_fixed<9,5,AP_RND,AP_SAT> data_t;
//typedef ap_fixed<16,8> data_t;
/*
void forward(const unsigned char src_data[N_INPUT],
                float dst_data[N_OUTPUT],
                float w01[N_INPUT*N_H1],
                float b1[N_H1],
                float w12[N_H1*N_H2],
                float b2[N_H2],
                float w23[N_H2*N_OUTPUT],
                float b3[N_OUTPUT]);
*/
void forward_fixed(const unsigned char src_data[N_INPUT],
                data_t dst_data[N_OUTPUT],
                const data_t w01[N_INPUT*N_H1],
                const data_t b1[N_H1],
                const data_t w12[N_H1*N_H2],
                const data_t b2[N_H2],
                const data_t w23[N_H2*N_OUTPUT],
                const data_t b3[N_OUTPUT]);

data_t relu_fixed(data_t a);

//float relu(float a);
void forward_cpu(const unsigned char src_data[N_INPUT],
                float dst_data[N_OUTPUT],
                float w01[N_INPUT*N_H1],
                float b1[N_H1],
                float w12[N_H1*N_H2],
                float b2[N_H2],
                float w23[N_H2*N_OUTPUT],
                float b3[N_OUTPUT]);
