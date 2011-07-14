/* draw.c */
#include <unistd.h>
#include <math.h>
#include <stdio.h>

#include "frame.h"

/*
 * draw a color line, the slope is single 
 */
int fb_draw_row(const fb_info fb_inf, int x, int y, int len, u32_t color)
{
    int iloop;
    
	  for (iloop = 0; iloop < len; iloop++)
	  {
		    fb_pixel(fb_inf, x + iloop, y, color);
		}
	  return 0;
}

/* 
* draw a color squarearea 
*/
int fb_draw_squarearea(const fb_info fb_inf, int x, int y, int x_len, int y_len, u32_t color)
{
    int iloop;
    int jloop;
    
    for (jloop = 0; jloop < y_len; jloop++)
    {
        for (iloop = 0; iloop < x_len; iloop++)
        {
            fb_pixel(fb_inf, x + iloop, y + jloop, color);
        }
    }
    return 0;
}

/* 
* draw a color ball 
*/
int fb_draw_ball(const fb_info fb_inf, int x, int y, int len, u32_t color)
{
    int iloop;
    int jloop;
    
    for (iloop = x - len; iloop <= x + len; iloop++)
    {
    	  for (jloop = y - len; jloop <= y + len; jloop++)
    	  {
    	  	  if ((iloop - x) * (iloop - x) + (jloop - y) * (jloop - y) <= len * len)
    	  	  {
    	  		    fb_pixel(fb_inf, iloop, jloop, color);
    	  		}
    	  }
    }
    return 0;
}

