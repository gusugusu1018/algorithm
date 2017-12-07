#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cuda.h>
#define N_X 100
#define N_Y 100
#define N_EPOCH 3
#define POINTS 1600

__global__ void calc_potential(const int *_ob_x,const int *_ob_y,const int p_x,const int p_y,const int g_x,const int g_y,float *U_d) {
   int i = blockDim.x*blockIdx.x+threadIdx.x;
   int j = blockDim.y*blockIdx.y+threadIdx.y;
   int k,m;
   for(k=0;k<=1000;k++) {
      /*
      for (m=0;m<POINTS;m++) {
         if (_ob_x[m]==i&&_ob_y[m]==j) U_d[i*N_X+j]=100.0f;
      }
      */
      if ((p_x==i&&p_y==j)|| i==0 || j==0 || i==(N_X-1) || j==(N_Y-1)) {
         U_d[i*N_X+j]=100.0f;
      } else if (g_x==i&&g_y==j) {
         U_d[i*N_X+j]=-100.0f;
      } else {
         U_d[i*N_X+j]=0.25f*(U_d[(i+1)*N_X+j]+U_d[(i-1)*N_X+j]+U_d[i*N_X+j+1]+U_d[i*N_X+j-1]);
      }
      __syncthreads();
   }
}

float U[N_X*N_Y]={0.0f};

double gettimeofday_sec(){
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec + tv.tv_usec * 1e-6;
}

void get_obstacle(int x[POINTS],int y[POINTS]);
void destination(int p_x,int p_y,int *min_x,int *min_y);

int main()
{
   int i,j,n,m;
   n=0;
   int ob_x[POINTS];
   int ob_y[POINTS];
   int p_x = 20;
   int p_y = 20;
   int g_x = 40;
   int g_y = 80;
   int dst_x;
   int dst_y;
   get_obstacle(ob_x,ob_y);
   FILE *potential;
   potential=fopen("potential.data","w");
   FILE *path;
   path=fopen("path.data","w");
   double start,end;

   int *_ob_x,*_ob_y;
   float *U_d;
   cudaMalloc((void**)&_ob_x,POINTS*sizeof(int));
   cudaMalloc((void**)&_ob_y,POINTS*sizeof(int));
   cudaMalloc((void**)&U_d,N_X*N_Y*sizeof(float));
   dim3 threads(32,32,1);
   dim3 blocks(ceil(N_X/threads.x),ceil(N_Y/threads.y),1);
   cudaMemcpy(_ob_x, ob_x, POINTS*sizeof(int), cudaMemcpyHostToDevice);
   cudaMemcpy(_ob_y, ob_y, POINTS*sizeof(int), cudaMemcpyHostToDevice);
   cudaMemcpy(U_d,U,N_X*N_Y*sizeof(float), cudaMemcpyHostToDevice);

   //   for (n=0;n<N_EPOCH;n++) {
   cudaDeviceSynchronize();
   start=gettimeofday_sec();

   calc_potential<<<blocks, threads>>>(_ob_x,_ob_y,p_x,p_y,g_x,g_y,U_d);

   cudaDeviceSynchronize();
   end=gettimeofday_sec();
   cudaMemcpy(U,U_d,N_X*N_Y*sizeof(float), cudaMemcpyDeviceToHost);
   cudaFree(_ob_x);
   cudaFree(_ob_y);
   cudaFree(U_d);
   cudaDeviceReset();
   printf("time\t%lf s\n", end-start);
   /*
   destination(p_x,p_y,&dst_x,&dst_y);
   if (U[dst_x*N_X+dst_y]>100.0f) {
      return 1;
   }
   fprintf(path,"%d %d %d\n",dst_x,dst_y,100-n);
   if ((dst_x==p_x)&&(dst_y==p_y)) break;
   p_x=dst_x;
   p_y=dst_y;
   */
   //   }
   /*
   for (m=0;m<POINTS;m++) U[ob_x[m]*N_X+ob_y[m]]=100.0f;
   U[p_x*N_X+p_y]=100.0f;
   U[g_x*N_X+g_y]=-100.0f;
   */
   for(i=0;i<N_X;i++) {
      for(j=0;j<N_Y;j++) {
         fprintf(potential,"%d %d %f\n",i,j,U[i*N_X+j]);
      }
      fprintf(potential,"\n");
   }
   fclose(potential);
   fclose(path);
   return 0;
}

void get_obstacle(int x[POINTS],int y[POINTS]) {
   int i;
   FILE *readfp;
   if ((readfp = fopen("POINTS.2d", "rb")) == NULL) {
      printf("Can't open a file\n");
      exit(1);
   }
   int x_buf;
   int y_buf;
   for (i=0;i<POINTS;i++) {
      fread(&x_buf,sizeof(int),1,readfp);
      fread(&y_buf,sizeof(int),1,readfp);
      x[i]=x_buf;
      y[i]=y_buf;
   }
   fclose(readfp);
}

void destination(int p_x,int p_y,int *min_x,int *min_y) {
   *min_x=p_x;
   *min_y=p_y;
   if (U[p_x*N_X+(p_y-1)]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x;
      *min_y=p_y-1;
   }
   if (U[p_x*N_X+(p_y+1)]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x;
      *min_y=p_y+1;
   }
   if (U[(p_x-1)*N_X+p_y]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x-1;
      *min_y=p_y;
   }
   if (U[(p_x+1)*N_X+p_y]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x+1;
      *min_y=p_y;
   }
   if (U[(p_x+1)*N_X+(p_y+1)]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x+1;
      *min_y=p_y+1;
   }
   if (U[(p_x+1)*N_X+(p_y-1)]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x+1;
      *min_y=p_y-1;
   }
   if (U[(p_x-1)*N_X+(p_y+1)]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x-1;
      *min_y=p_y+1;
   }
   if (U[(p_x-1)*N_X+(p_y-1)]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x-1;
      *min_y=p_y-1;
   }
   if (U[p_x*N_X+(p_y-2)]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x;
      *min_y=p_y-2;
   }
   if (U[p_x*N_X+(p_y+2)]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x;
      *min_y=p_y+2;
   }
   if (U[(p_x-2)*N_X+p_y]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x-2;
      *min_y=p_y;
   }
   if (U[(p_x+2)*N_X+p_y]<U[*min_x*N_X+*min_y]) {
      *min_x=p_x+2;
      *min_y=p_y;
   }
}
