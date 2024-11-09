#include <stdio.h>


int main( )
{
	int n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11;
	int odd, even;
	int check;

	printf("Enter the group of first twelve digits£º"); 
	n11 = getchar() - '0';
	n10 = getchar() - '0';	
	n9 = getchar() - '0';
	n8 = getchar() - '0';
	n7 = getchar() - '0';
	n6 = getchar() - '0';
	n5 = getchar() - '0';
	n4 = getchar() - '0';	
	n3 = getchar() - '0';
	n2 = getchar() - '0';
	n1 = getchar() - '0';
	n0 = getchar() - '0';
	
	odd = n11 + n9 + n7 + n5 + n3 + n1;
	even = n10 + n8 + n6 + n4 + n2 + n0;
	check = (10 - 1) - (even * 3 + odd - 1) % 10;
	
	printf("Check digit: %d",check);

	return 0;
}

