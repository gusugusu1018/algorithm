#include <list>
#include <vector>
#include <iostream>
#include "mnist.h"

using namespace std;

int main(){
	std::cout << "init dataset..." << std::endl;
	std::vector<std::vector<float> > train_data;
	std::vector<float> label_data;
	Mnist mnist;
	train_data = mnist.readTrainingFile("../data/train-images-idx3-ubyte");
	label_data = mnist.readLabelFile("../data/train-labels-idx1-ubyte");

	for (int k=0;k<5;k++) {
		for (int j=0;j<28;j++) {
			for (int i=0;i<28;i++) {
				
				if (train_data[k][i+j*28]!=0) {
					std::cout<<" "<<label_data[k];
				}else {
					std::cout<<"  ";
				}
				
				//std::cout <<" "<< train_data[k][i+j*28];
			}
			std::cout<<std::endl;
		}
	}
	return 0;
}

