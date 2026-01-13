// jiechen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define MAX_NUM 1000
#define STORE_SIZE  3000

int main(int argc, char* argv[])
{
	//Initialize the result and Set the last key
	int result[STORE_SIZE] = {1};
	
	//Begin compute the result
	for(int j = 1; j <= MAX_NUM; j++)
	{
		//Times each key with the new number
		for(i = 0; i < STORE_SIZE; i++)
		{
			result[i] *= j;
		}

        //We will add the 
		for(i = 0; i < STORE_SIZE; i++)
		{
			if(result[i] >= 10)
			{			
				//Add the result of 10 times to the high key
				result[i + 1] += result[i] / 10;

				//Get the value of current key.
				result[i] = result[i] % 10;
			}

		}
	}
	
	//print the result
	for(i = STORE_SIZE - 1; i >= 0; i--)
	{
		printf("%d",result[i]);
	}
    
    //pause
	getchar();
	return 0;
}

