#include <sstream>
#include <fstream>

#define N_INPUT 784
#define N_H1	1024
#define N_H2	1024
#define N_OUTPUT	10


int get_weights(float w01[N_INPUT][N_H1]
		,float b1[N_H1]
		,float w12[N_H1][N_H2]
		,float b2[N_H2]
		,float w23[N_H2][N_OUTPUT]
		,float b3[N_OUTPUT]
		) {
	std::ifstream ifs("initial_weights.csv");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 0;
	}
	std::cout<<"\n*******************get weights*****************"<<std::endl;
	std::string str;
	int m,x,y;
	float z;
	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		std::getline(stream,token,',');
		if ("w"==token) {
			for (int j=0;j<4;j++) {
				std::getline(stream,token,',');
				if (j==0) {
					// weight layer
					m=std::stoi(token);
				} else if (j==1) {
					// index
					x=std::stoi(token);
				} else if (j==2) {
					// index
					y=std::stoi(token);
				} else {
					// weight
					z=std::stod(token);
					if (m==1) {
						w01[x][y]=z;
					} else if (m==12) {
						w12[x][y]=z;
					} else if (m==23) {
						w23[x][y]=z;
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
					if (m==1) {
						b1[x]=z;
					} else if (m==2) {
						b2[x]=z;
					} else if (m==3) {
						b3[x]=z;
					}
				}
			}
		}
	}

	for (int i=0;i<N_INPUT;i++) {
		for (int j=0;j<N_H1;j++) {
			std::cout<<" w01["<<i<<"]["<<j<<"] = "<<w01[i][j];
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<N_H1;i++) {
		std::cout<<" b1["<<i<<"] = "<<b1[i];
	}
	std::cout<<std::endl;
	for (int i=0;i<N_H1;i++) {
		for (int j=0;j<N_H2;j++) {
			std::cout<<" w12["<<i<<"]["<<j<<"] = "<<w12[i][j];
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<N_H2;i++) {
		std::cout<<" b2["<<i<<"] = "<<b2[i];
	}
	std::cout<<std::endl;
	for (int i=0;i<N_H2;i++) {
		for (int j=0;j<N_OUTPUT;j++) {
			std::cout<<" w23["<<i<<"]["<<j<<"] = "<<w23[i][j];
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<N_OUTPUT;i++) {
		std::cout<<" b3["<<i<<"] = "<<b3[i];
	}
	std::cout<<std::endl;
	return 1;
}

int write_weights(float w01[N_INPUT][N_H1]
		,float b1[N_H1]
		,float w12[N_H1][N_H2]
		,float b2[N_H2]
		,float w23[N_H2][N_OUTPUT]
		,float b3[N_OUTPUT]
		) {
	std::ofstream ofs("new_weights.csv");
	for (int i=0;i<N_INPUT;i++) {
		for (int j=0;j<N_H1;j++) {
			std::cout<<" w01["<<i<<"]["<<j<<"] = "<<w01[i][j];
			ofs<<"w,1,"<<i<<","<<j<<","<<w01[i][j]<<std::endl;
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<N_H1;i++) {
		std::cout<<" b1["<<i<<"] = "<<b1[i];
		ofs<<"b,1,"<<i<<","<<b1[i]<<std::endl;
	}
	std::cout<<std::endl;
	for (int i=0;i<N_H1;i++) {
		for (int j=0;j<N_H2;j++) {
			std::cout<<" w12["<<i<<"]["<<j<<"] = "<<w12[i][j];
			ofs<<"w,12,"<<i<<","<<j<<","<<w12[i][j]<<std::endl;
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<N_H2;i++) {
		std::cout<<" b2["<<i<<"] = "<<b2[i];
		ofs<<"b,2,"<<i<<","<<b2[i]<<std::endl;
	}
	std::cout<<std::endl;
	for (int i=0;i<N_H2;i++) {
		for (int j=0;j<N_OUTPUT;j++) {
			std::cout<<" w23["<<i<<"]["<<j<<"] = "<<w23[i][j];
			ofs<<"w,23,"<<i<<","<<j<<","<<w23[i][j]<<std::endl;
		}
		std::cout<<std::endl;
	}
	for (int i=0;i<N_OUTPUT;i++) {
		std::cout<<" b2["<<i<<"] = "<<b3[i];
		ofs<<"b,3,"<<i<<","<<b3[i]<<std::endl;
	}
	std::cout<<std::endl;
	return 1;
}
