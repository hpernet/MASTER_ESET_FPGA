#include <stdio.h>

int N = 7;
uint8_t LIST[N] = {4;5;3;6;1;8;2};

void main (void)
{
	int counter = N; 
	int plusgrandevaleur = LIST[0]; 
	
	while(1)
	{
		for (int index =0; i<N; i++)
		{
			if(plusgrandevaleur < LIST[index])
			{
				plusgrandevaleur = LIST[index];
			}
		}
	}
}	
	 
