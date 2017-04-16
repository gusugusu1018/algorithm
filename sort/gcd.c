#include <stdio.h>
#include <stdlib.h>

int gcd(int a0, int a1);

int main()
{
	int a0, a1,temp;
	printf("Type in the first integer.\n");
	scanf("%d", &a0);
	printf("a0 = %d\n", a0);
	printf("Type in the Second integer.\n");
	scanf("%d", &a1);
	printf("a0 = %d\n", a1);
	if (a0 <= 0 || a1<= 0){
		printf("Illegal input number\n");
		exit(1);
	} if (a0 < a1){
		temp = a0;
		a0 = a1;
		a1 = temp;
	}
	printf("GCD = %d\n", gcd(a0,a1));
	return 0;
}

int gcd(int a0, int a1)
{
	int a, b, temp;
	a = a0;
	b = a1;
	while (b != 0)
	{
		temp = a%b;
		printf("temp = %d\n", temp);
		a = b;
		b = temp;
	}
	return a;
}

