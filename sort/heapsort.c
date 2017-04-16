#include <stdio.h>
#include <stdlib.h>

int inputfile(char *, int *);
void outputfile(int, int *);
void heapsor(int, int *);
void heapify(int *, int);
void downmax(int, int *, int);
int deletemax(int *, int);
void swap(int, int, int *);

int main(void)
{
	int A[256];
	char filename[32]="data2.txt";
	int i,n;

	n = inputfile(filename, A);
	heapsor(n, A);
	outputfile(n, A);
	return 0;
}

void heapsor(int n, int *A)
{
	int i;
	heapify(A,n);
	for (i=n-1;i>0;i--) {
		A[i]=deletemax(A, i+1);
	}
}

void heapify(int *A, int n)
{
	int i;
	for (i = n/2-1; i>=0; i--) {
		downmax(i, A, n);
	}
}

void downmax(int i, int *A, int n)
{
	int j;
	j=2*i+1;
	if (j>=n) return;
	if (j+1<n && A[j]<A[j+1]) j=j+1;
	if (A[j] > A[i]) {
		swap(i,j,A);
		downmax(j,A,n);
	}
	return;
}

int deletemax(int *A, int n)
{
	int max;
	max = A[0];
	A[0] =A[n-1];
	downmax(0,A,n-1);
	return max;
}

int inputfile(char *filename, int *A)
{
	FILE *fp;
	int n=0;
	int i;
	if ((fp = fopen(filename, "r")) == NULL) {
		printf("error!\nfile can't open\n");
		exit(0);
	} else {
		while ( ! feof(fp) && n < 256) {
			fscanf(fp, "%d", &(A[n]));
			n++;
		}
	}
	fclose(fp);
	n=n-2;
	printf("--input--\n");
	outputfile(n, A);
	return n;
}

void outputfile(int n, int *A)
{
	int i;

	printf("--output--\n");
	for(i=0;i<n+1;i++){
		printf("A[%d]=%d\n", i, A[i]);
	}
	return;
}

void swap(int a, int b, int *A)
{
	int temp;
	temp = A[a];
	A[a] = A[b];
	A[b] = temp;
}

