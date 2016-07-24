#include <stdio.h>
#include <stdlib.h>

int inputfile(char *, int *);
void outputfile(int, int *);
void bubblesort(int, int *);
void swap(int, int, int *);


int main(void)
{
	int A[256];
	char filename[32]="data.txt";
	int i,n;

	n = inputfile(filename, A);
	bubblesort(n, A);
	outputfile(n, A);
	return 0;
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
	n=n-2;
	fclose(fp);
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

void bubblesort(int n, int *A)
{
	int i,j;
	int test,counter=0;
	int temp;

	for (i=0; i<n; i++) {
		test = 1;
		counter++;
		for (j=n; j>=i+1; j--) {
			if (A[j] < A[j-1]) {
				swap(j, j-1, A);
				test = 0;
			}
		}
		if (test == 1) return;
		printf("counter = %d\n", counter);
		outputfile(n, A);
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

