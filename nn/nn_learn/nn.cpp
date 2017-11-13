#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#define NMAX	4
#define INPUTNO 2
#define HIDDENNO 3
#define OUTPUTNO 2
#define MAXINPUTNO 100

typedef struct Dataset {
	int sweetness;
	int sourness;
	int like[OUTPUTNO];
} dataset;

int get_data(dataset data[NMAX]) {
	std::ifstream ifs("data.csv");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 0;
	}
	std::cout<<"\n*******************get data*****************"<<std::endl;
	std::string str;
	for (int i=0;i<NMAX;i++) {
		std::getline(ifs,str);
		std::string token;
		std::istringstream stream(str);
		for (int j=0;j<4;j++) {
			std::getline(stream,token,',');
			if (j==0) {
				data[i].sweetness = std::stod(token);
				std::cout<<"data "<<i<<" :sweetness "<<data[i].sweetness;
			} else if (j==1) {
				data[i].sourness = std::stod(token);
				std::cout<<", sourness "<<data[i].sourness;
			} else if (j==2){
				data[i].like[0] = std::stoi(token);
			} else {
				data[i].like[1] = std::stoi(token);
				std::cout<<", like ["<<data[i].like[0]<<","<<data[i].like[1]<<"]"<<std::endl;
			}
		}
	}
	return 1;
}

int get_weights(double wh[INPUTNO][HIDDENNO]
		,double bh[HIDDENNO]
		,double wo[HIDDENNO][OUTPUTNO]
		,double bo[OUTPUTNO]
		) {
	std::ifstream ifs("initial_weights.csv");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 0;
	}
	std::cout<<"\n*******************get weights*****************"<<std::endl;
	std::string str;
	int m,x,y;
	double z;
	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		std::getline(stream,token,',');
		if ("w"==token) {
			for (int j=0;j<4;j++) {
				std::getline(stream,token,',');
				if (j==0) {
					m=std::stoi(token);
				} else if (j==1) {
					x=std::stoi(token);
				} else if (j==2) {
					y=std::stoi(token);
				} else {
					z=std::stod(token);
					if (m==2) {
						wh[x][y]=z;
					} else if (m==3) {
						wo[x][y]=z;
					}
				}
			}
		} else if ("b"==token) {
			for (int j=0;j<3;j++) {
				std::getline(stream,token,',');
				if (j==0) {
					m=std::stoi(token);
				} else if (j==1) {
					x=std::stoi(token);
				} else {
					z=std::stod(token);
					if (m==2) {
						bh[x]=z;
					} else if (m==3) {
						bo[x]=z;
					}
				}
			}
		}
	}

	for (int i=0;i<INPUTNO;i++) {
		for (int j=0;j<HIDDENNO;j++) {
			std::cout<<" wh["<<i<<"]["<<j<<"] = "<<wh[i][j];
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<HIDDENNO;i++) {
		std::cout<<" bh["<<i<<"] = "<<bh[i];
	}
	std::cout<<std::endl;
	for (int i=0;i<HIDDENNO;i++) {
		for (int j=0;j<OUTPUTNO;j++) {
			std::cout<<" wo["<<i<<"]["<<j<<"] = "<<wo[i][j];
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<OUTPUTNO;i++) {
		std::cout<<" bo["<<i<<"] = "<<bo[i];
	}
	std::cout<<std::endl;
	return 1;
}
int write_weights(double wh[INPUTNO][HIDDENNO]
		,double bh[HIDDENNO]
		,double wo[HIDDENNO][OUTPUTNO]
		,double bo[OUTPUTNO]
		) {
	std::ofstream ofs("new_weights.csv");
	for (int i=0;i<INPUTNO;i++) {
		for (int j=0;j<HIDDENNO;j++) {
			std::cout<<" wh["<<i<<"]["<<j<<"] = "<<wh[i][j];
			ofs<<"w,2,"<<i<<","<<j<<","<<wh[i][j]<<std::endl;
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<HIDDENNO;i++) {
		std::cout<<" bh["<<i<<"] = "<<bh[i];
		ofs<<"b,2,"<<i<<","<<bh[i]<<std::endl;
	}
	std::cout<<std::endl;
	for (int i=0;i<HIDDENNO;i++) {
		for (int j=0;j<OUTPUTNO;j++) {
			std::cout<<" wo["<<i<<"]["<<j<<"] = "<<wo[i][j];
			ofs<<"w,3,"<<i<<","<<j<<","<<wo[i][j]<<std::endl;
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<OUTPUTNO;i++) {
		std::cout<<" bo["<<i<<"] = "<<bo[i];
		ofs<<"b,3,"<<i<<","<<bo[i]<<std::endl;
	}
	std::cout<<std::endl;
	return 1;
}

double sigmoid(double u)
{
	return 1.0/(1.0+exp(-u));
}

void forward(dataset data
		,double wh[INPUTNO][HIDDENNO]
		,double bh[HIDDENNO]
		,double zh[HIDDENNO]
		,double ah[HIDDENNO]
		,double wo[HIDDENNO][OUTPUTNO]
		,double bo[OUTPUTNO]
		,double zo[OUTPUTNO]
		,double ao[OUTPUTNO]
	    ) {
	for (int i=0;i<HIDDENNO;i++) {
		zh[i]=wh[0][i]*data.sweetness+wh[1][i]*data.sourness+bh[i];
		ah[i]=sigmoid(zh[i]);
		std::cout<<" ah["<<i<<"] "<<ah[i];
	}
	std::cout<<std::endl;
	for (int i=0;i<OUTPUTNO;i++) {
		zo[i]=wo[0][i]*ah[0]+wo[1][i]*ah[1]+wo[2][i]*ah[2]+bo[i];
		ao[i]=sigmoid(zo[i]);
		std::cout<<" ao["<<i<<"] "<<ao[i];
	}
	std::cout<<std::endl;
	double cost=(pow(ao[0]-data.like[0],2)+pow(ao[1]-data.like[1],2))*0.5;
	std::cout<<"cost : "<<cost<<std::endl;
}

double diff_sigmoid(double u)
{
	return (1.0-sigmoid(u))*sigmoid(u);
}

void backward(dataset data
		,double wh[INPUTNO][HIDDENNO]
		,double bh[HIDDENNO]
		,double zh[HIDDENNO]
		,double ah[HIDDENNO]
		,double wo[HIDDENNO][OUTPUTNO]
		,double bo[OUTPUTNO]
		,double zo[OUTPUTNO]
		,double ao[OUTPUTNO]
		,double deltah[HIDDENNO]
		,double deltao[OUTPUTNO]
	    ) {
	forward(data,wh,bh,zh,ah,wo,bo,zo,ao);
	for (int i=0;i<OUTPUTNO;i++) {
		deltao[i]=(ao[i]-data.like[i])*diff_sigmoid(zo[i]);
		std::cout<<" deltao["<<i<<"] : "<<deltao[i];
	}
	std::cout<<std::endl;
	/*
	deltao[0]=(ao[0]-data.like[0])*diff_sigmoid(zo[0]);
	deltao[1]=(ao[1]-data.like[1])*diff_sigmoid(zo[1]);
	std::cout<<"deltao[0] : "<<deltao[0]<<std::endl;
	*/
	for (int i=0;i<HIDDENNO;i++) {
		deltah[i]=(deltao[0]*wo[i][0]+deltao[1]*wo[i][1])*diff_sigmoid(zh[i]);
		std::cout<<" deltah["<<i<<"] : "<<deltah[i];
	}
	std::cout<<std::endl;
	/*
	deltah[0]=(deltao[0]*wo[0][0]+deltao[1]*wo[0][1])*diff_sigmoid(zh[0]);
	deltah[1]=(deltao[0]*wo[1][0]+deltao[1]*wo[1][1])*diff_sigmoid(zh[1]);
	deltah[2]=(deltao[0]*wo[2][0]+deltao[1]*wo[2][1])*diff_sigmoid(zh[2]);
	*/
}

void gradient_descent_solver(double eta
		,dataset data[NMAX]
		,double wh[INPUTNO][HIDDENNO]
		,double bh[HIDDENNO]
		,double ah[NMAX][HIDDENNO]
		,double wo[HIDDENNO][OUTPUTNO]
		,double bo[OUTPUTNO]
		,double ao[NMAX][OUTPUTNO]
		,double deltah[NMAX][HIDDENNO]
		,double deltao[NMAX][OUTPUTNO]
	    ) {
	double Delta_wo[HIDDENNO][OUTPUTNO];
	for (int j=0;j<HIDDENNO;j++) {
		for (int i=0;i<OUTPUTNO;i++) {
			Delta_wo[j][i]=0.0;
		}
	}
	for (int j=0;j<HIDDENNO;j++) {
		for (int i=0;i<OUTPUTNO;i++) {
			for (int n=0;n<NMAX;n++) {
				Delta_wo[j][i]+=deltao[n][i]*ah[n][j];
			}
			std::cout<<" Delta_wo["<<j<<"]["<<i<<"]="<<Delta_wo[j][i]<<" ,wo_old["<<j<<"]["<<i<<"]="<<wo[j][i];
			wo[j][i] = wo[j][i]-eta*Delta_wo[j][i];
			std::cout<<" ,wo_new["<<j<<"]["<<i<<"]="<<wo[j][i]<<std::endl;
		}
	}
	/*
	Delta_wo[0][0]=deltao[0][0]*ah[0][0]+deltao[1][0]*ah[1][0]+deltao[2][0]*ah[2][0]+deltao[3][0]*ah[3][0];
	Delta_wo[0][1]=deltao[0][1]*ah[0][0]+deltao[1][1]*ah[1][0]+deltao[2][1]*ah[2][0]+deltao[3][1]*ah[3][0];
	Delta_wo[1][0]=deltao[0][0]*ah[0][1]+deltao[1][0]*ah[1][1]+deltao[2][0]*ah[2][1]+deltao[3][0]*ah[3][1];
	Delta_wo[1][1]=deltao[0][1]*ah[0][1]+deltao[1][1]*ah[1][1]+deltao[2][1]*ah[2][1]+deltao[3][1]*ah[3][1];
	Delta_wo[2][0]=deltao[0][0]*ah[0][2]+deltao[1][0]*ah[1][2]+deltao[2][0]*ah[2][2]+deltao[3][0]*ah[3][2];
	Delta_wo[2][1]=deltao[0][1]*ah[0][2]+deltao[1][1]*ah[1][2]+deltao[2][1]*ah[2][2]+deltao[3][1]*ah[3][2];
	*/
	double Delta_bo[OUTPUTNO];
	for (int i=0;i<OUTPUTNO;i++) {
		Delta_bo[i]=0.0;
	}
	for (int i=0;i<OUTPUTNO;i++) {
		for (int n=0;n<NMAX;n++) {
			Delta_bo[i]+=deltao[n][i];
		}
		std::cout<<" Delta_bo["<<i<<"]="<<Delta_bo[i]<<" ,bo_old["<<i<<"]="<<bo[i];
		bo[i]=bo[i]-eta*Delta_bo[i];
		std::cout<<" ,bo_new["<<i<<"]="<<bo[i]<<std::endl;
	}
	/*
	Delta_bo[0]=deltao[0][0]+deltao[1][0]+deltao[2][0]+deltao[3][0];
	Delta_bo[1]=deltao[0][1]+deltao[1][1]+deltao[2][1]+deltao[3][1];
	*/

	double Delta_wh[INPUTNO][HIDDENNO];
	for (int j=0;j<INPUTNO;j++) {
		for (int i=0;i<HIDDENNO;i++) {
			Delta_wh[j][i]=0.0;
		}
	}
	for (int j=0;j<INPUTNO;j++) {
		for (int i=0;i<HIDDENNO;i++) {
			for (int n=0;n<NMAX;n++) {
				if (j==0) {
					Delta_wh[j][i]+=deltah[n][i]*data[n].sweetness;
				} else {
					Delta_wh[j][i]+=deltah[n][i]*data[n].sourness;
				}
			}
			std::cout<<" Delta_wh["<<j<<"]["<<i<<"]="<<Delta_wh[j][i]<<" ,wh_old["<<j<<"]["<<i<<"]="<<wh[j][i];
			wh[j][i] = wh[j][i]-eta*Delta_wh[j][i];
			std::cout<<" ,wh_new["<<j<<"]["<<i<<"]="<<wh[j][i]<<std::endl;
		}
	}

	double Delta_bh[HIDDENNO];
	for (int i=0;i<HIDDENNO;i++) {
		Delta_bh[i]=0.0;
	}
	for (int i=0;i<HIDDENNO;i++) {
		for (int n=0;n<NMAX;n++) {
			Delta_bh[i]+=deltah[n][i];
		}
		std::cout<<" Delta_bh["<<i<<"]="<<Delta_bh[i]<<" ,bh_old["<<i<<"]="<<bh[i];
		bh[i]=bh[i]-eta*Delta_bh[i];
		std::cout<<" ,bh_new["<<i<<"]="<<bh[i]<<std::endl;
	}
}

int main()
{
	double wh[INPUTNO][HIDDENNO];
	double bh[HIDDENNO];
	double zh[HIDDENNO];
	double ah[NMAX][HIDDENNO];
	double deltah[NMAX][HIDDENNO];

	double wo[HIDDENNO][OUTPUTNO];
	double bo[OUTPUTNO];
	double zo[OUTPUTNO];
	double ao[NMAX][OUTPUTNO];
	double deltao[NMAX][OUTPUTNO];

	dataset data[NMAX];
	double eta=0.1;

	if (get_data(data)==0) {
		std::cout<<"input error"<<std::endl;
		return 0;
	}

	if (get_weights(wh,bh,wo,bo)==0) {
		std::cout<<"input error"<<std::endl;
		return 0;
	}

        for (int n=0;n<NMAX;n++) {
                std::cout<<"n="<<n<<std::endl;
                backward(data[n],wh,bh,zh,ah[n],wo,bo,zo,ao[n],deltah[n],deltao[n]);
        }


	gradient_descent_solver(eta,data,wh,bh,ah,wo,bo,ao,deltah,deltao);
	write_weights(wh,bh,wo,bo);

	return 0 ;
}
