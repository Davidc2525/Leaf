#include <math.h>
#include <stdio.h>
#include <unistd.h>

typedef int(fun)();

void print_float(int cc, float n, float n2, int csm)
{
	float a;
	float b;

	a = n;
	b = n2;
	b = a + b;
	//printf("%f\n",a);
}

void nc()
{
	print_float(5, 2.5, 1.5, 2);
}
int escribirln(char *str)
{
	return printf("%s\n", str);
}

int jjj()
{
	return printf("jjj\n");
}
void *ggg()
{
	return jjj;
}

int csmc(char *(mc)())
{
	return escribirln(mc());
}

int promedio3c(int a, int b, int c)
{
	printf("%d %d %d\n", a, b, c);
	return (a + b + c) / 3;
}

int *pun(int *a)
{
	*a = 100;
	return a;
}

int suma(int a, int b)
{

	return a + b;
}

int mod(int a, int b)
{

	return a % b;
}
