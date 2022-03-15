// INCLUDE ---------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <stdbool.h>

// DEFINE ---------------------------------------------------------------------------------------------------
#define PIXELMAP_BASE_ADRESS	(volatile uint16_t*)	0x08000000
#define STATUS_REGISTER 		(volatile uint16_t*)	0x1000302C
#define FRONT_BUFFER_REGISTER   (volatile uint16_t*)	0x10003020
#define X_MAX                  320 
#define Y_MAX                  240
#define COLOR_BLACK            0x0000
#define COLOR_WHITE            0xFFFF
#define COLOR_RED              0xF800
#define COLOR_PURPLE           0xF81F
#define COLOR_GRAY             0x8410
#define COLOR_GREEN            0x7E00
#define COLOR_BLUE             0x001F
#define NB_COLOR               6
#define NB_OF_NODE             10

// FUNCTIONS PROTOTYPE --------------------------------------------------------------------------------------
void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void draw_background(uint16_t color);
void draw_line(int x0, int x1, int y0, int y1, int color);

typedef struct 
{
	int coord_x;
	int coord_y;
	
	int sens_x;
	int sens_y;
} node_t;

// VARIABLES  -----------------------------------------------------------------------------------------------
int index	= 0;
int p_index	= Y_MAX-1;
int node_index;
int color_array[NB_COLOR] = {0xFFFF, 0xF800, 0xF81F, 0x8410, 0x7E00, 0x001F};

node_t node_array[NB_OF_NODE];

// MAIN -----------------------------------------------------------------------------------------------------
int main(void)
{
	// INITIALIZE
	draw_background(COLOR_BLACK);
	
	for(node_index = 0; node_index < NB_OF_NODE; node_index++)
	{
		node_array[node_index].coord_x = rand() % X_MAX;
		node_array[node_index].coord_y = rand() % Y_MAX;
	}
	
	while(1)
	{
		// Wait next write cycle
		*FRONT_BUFFER_REGISTER = 1;
		while ((*STATUS_REGISTER & 1) !=0);
		
		
		// ERASE figure
		for(node_index = 0; node_index < NB_OF_NODE; node_index++)
		{
			draw_line(node_array[node_index].coord_x, 
					  node_array[(node_index + 1) % NB_OF_NODE].coord_x, 
					  node_array[node_index].coord_y, 
					  node_array[(node_index + 1) % NB_OF_NODE].coord_y, 
					  COLOR_BLACK);
		}
		
		// Incremente node coord
		for(node_index = 0; node_index < NB_OF_NODE; node_index++)
		{
			if (node_array[node_index].sens_x == 1)
			{
				if(node_array[node_index].coord_x >= X_MAX)
				{
					node_array[node_index].sens_x = 0;
				}
				else 
				{
					node_array[node_index].coord_x++;
				}
			}
			else 
			{
				if(node_array[node_index].coord_x <= 0)
				{
					node_array[node_index].sens_x = 1;
				}
				else 
				{
					node_array[node_index].coord_x--;
				}
			}
			
			if (node_array[node_index].sens_y == 1)
			{
				if(node_array[node_index].coord_y >= Y_MAX)
				{
					node_array[node_index].sens_y = 0;
				}
				else 
				{
					node_array[node_index].coord_y++;
				}
			}
			else 
			{
				if(node_array[node_index].coord_y <= 0)
				{
					node_array[node_index].sens_y = 1;
				}
				else 
				{
					node_array[node_index].coord_y--;
				}
			}
		}
		
		// Draw figure
		for(node_index = 0; node_index < NB_OF_NODE; node_index++)
		{
			draw_line(node_array[node_index].coord_x, 
					  node_array[(node_index + 1) % NB_OF_NODE].coord_x, 
					  node_array[node_index].coord_y, 
					  node_array[(node_index + 1) % NB_OF_NODE].coord_y, 
					  color_array[node_index % NB_COLOR]);
		}
	}
}

// FUNCTIONS ------------------------------------------------------------------------------------------------
void draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	uint32_t offset;
	
	offset = x + (y << 9);
	
	*(PIXELMAP_BASE_ADRESS + offset) = color;
}
void draw_background(uint16_t color)
{
	uint16_t ibcly, ibclx;
	
	for (ibclx = 0; ibclx < X_MAX; ibclx++)
	{
		for (ibcly = 0; ibcly < Y_MAX; ibcly++)
		{
			draw_pixel(ibclx, ibcly, color);
		}
	}
}
	
void draw_line(int x0, int x1, int y0, int y1, int color)
{
	uint16_t m_x1, m_y0;
	uint8_t is_step;
	uint16_t x;
	int deltax;
	int deltay;
	int error;
	int y;
	int y_step;

	
	if (abs(y1 - y0) > abs(x1 - x0))
	{
		is_step = 1;
	}
	else 
	{
		is_step = 0;
	}
	
	if(is_step)
	{
		// swap(x0, y0)
		m_y0= y0;
		y0	= x0;
		x0	= m_y0;
		
		//swap(x1, y1)
		m_x1= x1;
		x1	= y1;
		y1	= m_x1;
	}
	if(x0 > x1)
	{
		// swap(x0, x1)
		m_x1= x1;	
		x1	= x0;
		x0	= m_x1;
		
		// swap(y0, y1)
		m_y0= y0;
		y0	= y1;
		y1	= m_y0;
	}
	
	deltax = x1 - x0;
	deltay = abs(y1 - y0);
	error = -(deltax / 2);
	y = y0;
	
	if(y0 < y1)
	{
		y_step = 1;
	}
	else
	{
		y_step = -1;
	}
	
	for(x = x0; x < x1; x++)
	{
		if(is_step)
		{
			draw_pixel(y, x, color);
		}
		else
		{
			draw_pixel(x, y, color);
		}
		error = error + deltay;
		if(error >= 0)
		{
			y = y + y_step;
			error = error - deltax;
		}
	}
}

// END OF FILE ----------------------------------------------------------------------------------------------
