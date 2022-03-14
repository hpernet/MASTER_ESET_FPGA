#include <stdio.h>
#include <stdlib.h> 

void draw_line(int x0, int x1, int y0, int y1)
{
	int m_x1, m_y0;
	
	boolean is_step = abs(y1 - y0) > abs(x1 - x0);
	if(is_step)
	{
		m_y0= y0;
		y0	= x0;
		x0	= m_y0;
		
		m_x1= x1;
		x1	= y1;
		y1	= m_x1;
	}
	if(x0 > x1)
	{
		m_x1= x1;
		x1	= x0;
		x0	= m_x1;
		
		m_y0= y0;
		y0	= y1;
		y1	= m_y0;
	}
	
	int deltax	= x1 - x0;
	int deltay	= abs(y1 - y0);
	int error	= -(deltax / 2);
	int y = y0;
	
	if(y0 < y1)
	{
		y_step = 1
	}
	else
	{
		y_step = -1;
	}
	
	for(int x=x0; x<x1; x++)
	{
		if(is_step)
		{
			draw pixel(y,x);
		}
		else
		{
			draw pixel(x,y);
		}
		error = error + deltay
		if(error >= 0)
		{
			y = y + y step;
			error = error – deltax;
		}
	}
}