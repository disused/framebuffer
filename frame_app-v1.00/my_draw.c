/* draw.c */
#include <unistd.h>
#include <math.h>
#include <stdio.h>

#include "frame.h"


/* draw a color circle */
int fb_draw_cir(const fb_info fb_inf, int x, int y, int len, u32_t color)
{
	int i, j;

	for (i = x - len ; i <= x + len ; i++)
	{
		for (j = y - len ; j <= y + len ; j++)
		{
			if ((i - x) * (i - x) + (j - y) * (j - y) <= (len)*(len) && (i - x) * (i - x) + (j - y) * (j - y) >= (len-1)*(len-1))
				fb_pixel(fb_inf, i, j, color);	
		}
	}

	return 0;
}

/*draw a ellipse*/
int fb_draw_ellipse(const fb_info fb_inf, int x, int y, int high, int width, u32_t color)
{
	int i, j;
	int a = width*width;
	int b = high*high;

	for (i = x - width ; i <= x + width ; i++)
	{
		for (j = y - high ; j <= y + high ; j++)
		{
			if (((i - x) * (i - x) * b + (j - y) * (j - y) *  a <= a*b)  && ((i - x) * (i - x) * b + (j - y) * (j - y) * a >= ((width-1)*(width-1))*((high-1)*(high-1))))
				fb_pixel(fb_inf, i, j, color);	
		}
	}

	return 0;
}

/*draw a upline*/
int fb_draw_upline(fb_info fb_inf, int x,int y, int len, u32_t color)
{
	int i,j;
	j = y;
	for(i=0; i<len; i++,j++)
	{
		fb_pixel(fb_inf,x, j, color);
	}
	

	return 0;
}

/*draw a rectangle*/
int fb_draw_rect(fb_info fb_inf, int x,int y, int high, int width, u32_t color)
{
	int i;
	for(i = 0; i < width; i++)
	{
		fb_pixel(fb_inf,x+i, y, color);
	}
	for(i = 1; i < high; i++)
	{
		fb_pixel(fb_inf, x, y+i, color);
		fb_pixel(fb_inf, x+width, y+i, color);
	}
	for(i = 0; i < width; i++)
	{
		fb_pixel(fb_inf,x+i, y+high, color);
	}

	return 0;
}

int fb_draw_slopline(fb_info fb_inf, int x,int y, int len, u32_t color)
{
	int i,j;
	int p = len/2;
	
	
	for(i=0;i<p;i++)
	{
		fb_pixel(fb_inf,x+i, y+i, color);
	}
	return 0;
}
