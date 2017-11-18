#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#define NMAX	4
#define INPUTNO 2
#define HIDDENNO 3
#define OUTPUTNO 2
#define MAXINPUTNO 100

void initwh(double wh[INPUTNO][HIDDENNO])
{
	wh[0][0]=0.1;
	wh[0][1]=0.3;
	wh[0][2]=0.6;
	wh[1][0]=0.4;
	wh[1][1]=0.5;
	wh[1][2]=0.1;
} 

void initwo(double wo[HIDDENNO][OUTPUTNO])
{
	wo[0][0]=0.7;
	wo[1][0]=0.2;
	wo[2][0]=1.3;
	wo[0][1]=0.2;
	wo[1][1]=0.5;
	wo[2][1]=1.1;
} 

typedef struct Dataset {
	int sweetness;
	int sourness;
	int like;
} dataset;

int get_data(dataset data[NMAX]) {
	std::ifstream ifs("data.csv");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 0;
	}
	std::string str;
	for (int i=0;i<NMAX;i++) {
		std::getline(ifs,str);
		std::string token;
		std::istringstream stream(str);
		for (int j=0;j<3;j++) {
			std::getline(stream,token,',');
			if (j==0) {
				data[i].sweetness = std::stof(token);
				std::cout<<"data"<<i<<":sweetness "<<data[i].sweetness;
			} else if (j==1) {
				data[i].sourness = std::stof(token);
				std::cout<<", sourness "<<data[i].sourness;
			} else {
				data[i].like = std::stof(token);
				std::cout<<", like "<<data[i].like<<std::endl;
			}
		}
	}
	return 1;
}

double sigmoid(double u)
{
	return 1.0/(1.0+exp(-u));
}

void forward(dataset data
		,double wh[INPUTNO][HIDDENNO]
		,double bh[HIDDENNO]
		,double ah[HIDDENNO]
		,double wo[HIDDENNO][OUTPUTNO]
		,double bo[OUTPUTNO]
		,double ao[OUTPUTNO]
	    ) {
	for (int i=0;i<HIDDENNO;i++) {
		ah[i]=sigmoid(wh[0][i]*data.sweetness+wh[1][i]*data.sourness+bh[i]);
		std::cout<<" ah["<<i<<"] "<<ah[i];
	}
	std::cout<<std::endl;
	for (int i=0;i<OUTPUTNO;i++) {
		ao[i]=sigmoid(wo[0][i]*ah[0]+wo[1][i]*ah[1]+wo[2][i]*ah[2]+bo[i]);
		std::cout<<" ao["<<i<<"] "<<ao[i];
	}
	std::cout<<std::endl;
}

int main()
{
	double wh[INPUTNO][HIDDENNO];
	initwh(wh);
	double bh[HIDDENNO]={-1,-1,-1};
	double ah[HIDDENNO];

	double wo[HIDDENNO][OUTPUTNO];
	initwo(wo);
	double bo[OUTPUTNO]={-1,-1};
	double ao[OUTPUTNO];

	dataset data[NMAX];

	if (get_data(data)==0) {
		std::cout<<"input error"<<std::endl;
		return 0;
	}

	for (int n=0;n<NMAX;n++) {
		std::cout<<"n="<<n<<std::endl;
		forward(data[n],wh,bh,ah,wo,bo,ao);
		/*
		   for (int i=0;i<HIDDENNO;i++) {
		   ah[i]=sigmoid(wh[0][i]*data[n].sweetness+wh[1][i]*data[n].sourness+bh[i]);
		   std::cout<<" ah["<<i<<"] "<<ah[i];
		   }
		   std::cout<<std::endl;
		   for (int i=0;i<OUTPUTNO;i++) {
		   ao[i]=sigmoid(wo[0][i]*ah[0]+wo[1][i]*ah[1]+wo[2][i]*ah[2]+bo[i]);
		   std::cout<<" ao["<<i<<"] "<<ao[i];
		   }
		   std::cout<<std::endl;
		 */
	}
	/*
	//forward
	zh[0]=wh[0][0]*data.sweetness+wh[1][0]*data.sourness+bh[0];
	ah[0]=sigmoid(zh[0]);
	zh[1]=wh[0][1]*data.sweetness+wh[1][1]*data.sourness+bh[1];
	ah[1]=sigmoid(zh[1]);
	zh[2]=wh[0][2]*data.sweetness+wh[1][2]*data.sourness+bh[2];
	ah[2]=sigmoid(zh[2]);

	zo[0]=wo[0][0]*ah[0]+wo[1][0]*ah[1]+wo[2][0]*ah[2]+bo[0];
	ao[0]=sigmoid(zo[0]);
	zo[1]=wo[0][1]*ah[0]+wo[1][1]*ah[1]+wo[2][1]*ah[2]+bo[1];
	ao[1]=sigmoid(zo[1]);
	 */
	return 0 ;
}
