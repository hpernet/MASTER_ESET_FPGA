// INCLUDE ---------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// DEFINE ---------------------------------------------------------------------------------------------------
#define FRONT_BUFFER_ADRESS	    	(volatile uint16_t*) 0x08000000
#define BACK_BUFFER_ADRESS	    	(volatile uint16_t*) 0x08040000
#define STATUS_REGISTER 		(volatile uint16_t*)	0x1000302C
#define FRONT_BUFFER_REGISTER   	(volatile uint16_t*) 0x10003020
#define BACK_BUFFER_REGISTER    	(volatile uint16_t*) 0x10003024
#define X_MAX                  320 
#define Y_MAX                  240
#define COLOR_BLACK            0x0000
#define COLOR_WHITE            0xFFFF
#define COLOR_RED              0xF800
#define COLOR_PURPLE           0xF81F
#define COLOR_GRAY             0x8410
#define COLOR_GREEN            0x7E00
#define COLOR_BLUE             0x001F
#define NB_COLOR               16
#define NB_OF_NODE             16
#define INCREMENT              1

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
int color_array[NB_COLOR] = {0xFFFF, 0x0000, 0xF800, 0x07E0, 0x001F, 0xFFE0, 0x07FF, 0xF81F, 0xC618, 0x8410, 0x8000, 0x8400, 0x0400, 0x8010, 0x0410, 0x0010};

volatile uint16_t* p_front_buffer;
volatile uint16_t* p_back_buffer;

node_t node_array[NB_OF_NODE];

// MAIN -----------------------------------------------------------------------------------------------------
int main(void)
{
	p_front_buffer = FRONT_BUFFER_ADRESS;
	p_back_buffer  = BACK_BUFFER_ADRESS;
	
	draw_background(COLOR_BLACK);
	
	swapBufferRegister();
	
	draw_background(COLOR_BLACK);
	
	for(node_index = 0; node_index < NB_OF_NODE; node_index++)
	{
		node_array[node_index].coord_x = rand() % X_MAX;
		node_array[node_index].coord_y = rand() % Y_MAX;
	}
		
	while(1)
	{
		int i;

		for(i=0; i<256; i++)
		{
			swapBufferRegister();
			drawrect();
		}
	
		swapBufferRegister();
		
		for(i=0; i<NB_COLOR; i++)
		{
			draw_background(color_array[i]);
			swapBufferRegister();
		}
		
		/*
		// ERASE figure
		for(node_index = 0; node_index < NB_OF_NODE; node_index++)
		{
			draw_line(node_array[node_index].coord_x, 
					  node_array[(node_index + 1) % NB_OF_NODE].coord_x, 
					  node_array[node_index].coord_y, 
					  node_array[(node_index + 1) % NB_OF_NODE].coord_y, 
					  COLOR_BLACK);
		}
		swapBufferRegister();

		calculImage();
		
		// Draw figure
		for(node_index = 0; node_index < NB_OF_NODE; node_index++)
		{
			draw_line(node_array[node_index].coord_x, 
					  node_array[(node_index + 1) % NB_OF_NODE].coord_x, 
					  node_array[node_index].coord_y, 
					  node_array[(node_index + 1) % NB_OF_NODE].coord_y, 
					  color_array[node_index % NB_COLOR]);
		}
		swapBufferRegister();
		*/
	}
}

// FUNCTIONS ------------------------------------------------------------------------------------------------
void drawrect()
{
	int rnd_x1 = rand() % X_MAX;
	int rnd_x2 = rand() % X_MAX;
	int rnd_y1 = rand() % Y_MAX;
	int rnd_y2 = rand() % Y_MAX;
	
	draw_line(rnd_x1, rnd_x2, rnd_y1, rnd_y1, color_array[rand()% NB_COLOR]);
	draw_line(rnd_x1, rnd_x2, rnd_y2, rnd_y2, color_array[rand()% NB_COLOR]);
	draw_line(rnd_x1, rnd_x1, rnd_y1, rnd_y2, color_array[rand()% NB_COLOR]);
	draw_line(rnd_x2, rnd_x2, rnd_y1, rnd_y2, color_array[rand()% NB_COLOR]);
}
void calculImage()
{
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
				node_array[node_index].coord_x += INCREMENT;
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
				node_array[node_index].coord_x -= INCREMENT;
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
				node_array[node_index].coord_y += INCREMENT;
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
				node_array[node_index].coord_y -= INCREMENT;
			}
		}
	}
}

void swapBufferRegister()
{
	*FRONT_BUFFER_REGISTER = 1;	
	p_front_buffer = BACK_BUFFER_ADRESS;
	p_back_buffer  = FRONT_BUFFER_ADRESS;
	while ((*STATUS_REGISTER & 1) !=0);
}

void draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	uint32_t offset;
	
	offset = x + (y << 9);
	
	*(p_back_buffer + offset) = color;
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
