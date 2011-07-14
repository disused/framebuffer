/* draw-triangle.c */
#include <unistd.h>
#include <math.h>
#include <stdio.h>

#include "frame.h"

/*
* draw a triangle 
*/
int fb_draw_triangle(const fb_info fb_inf, int x, int y, int len, u32_t color)
{
    double x_len;
    double y_len;
    double y_curf;
    double y_curl;
    double iloop;
    double jloop;
    double drt_a = 0.0;
    
    double a_0 = (0.0 + drt_a) / 360 * 2 * 3.1415927;
    double a_18_r = (18.0 - drt_a) / 360 * 2 * 3.1415927;
    double a_54_r = (54.0 + drt_a) / 360 * 2 * 3.1415927;
    double a_18_l = (18.0 + drt_a) / 360 * 2 * 3.1415927;
    double a_54_l = (54.0 - drt_a) / 360 * 2 * 3.1415927;
    double a_36 = (36.0 + drt_a) / 360 * 2 * 3.1415927;
    double a_72 = (72.0 + drt_a) / 360 * 2 * 3.1425927;
    double a_144 = (144.0 + drt_a) / 360 * 2 * 3.1415927;
    double a_108 = (108.0 + drt_a) / 360 * 2 * 3.1415927;
    
    x_len = cos(a_54_r) * len;
    y_len = sin(a_54_r) * len;
    for (iloop = x; iloop <= x + x_len; iloop++)
    {
        y_curf = tan(a_72) * (iloop - x - x_len) + y + y_len;
        y_curl = tan(a_36) * (iloop - x - x_len) + y + y_len;
        for (jloop = y_curf; jloop <= y_curl; jloop++)
        {
            fb_pixel(fb_inf, iloop, jloop, color);
        }
    }
    
    x_len = cos(a_54_l) * len;
    y_len = sin(a_54_l) * len;
    for (iloop = x - x_len; iloop < x; iloop++)
    {
    	  y_curf = tan(a_108) * (iloop - x + x_len) + y + y_len;
    	  y_curl = tan(a_144) * (iloop - x + x_len) + y + y_len;
    	  for (jloop = y_curf; jloop <= y_curl; jloop++)
    	  {
    	      fb_pixel(fb_inf, iloop, jloop, color);
    	  }
    }
    
    x_len = cos(a_18_l) * len;
    y_len = sin(a_18_l) * len;
    for (iloop = x - x_len; iloop < x; iloop++)
    {
        y_curf = tan(a_0) * (iloop - x + x_len) + y - y_len;
        y_curl = tan(a_36) * (iloop - x + x_len) + y - y_len;
        for (jloop = y_curf; jloop <= y_curl; jloop++)
        {
            fb_pixel(fb_inf, iloop, jloop, color);
        }
    }
    
    x_len = cos(a_18_r) * len;
    y_len = sin(a_18_r) * len;
    for (iloop = x; iloop <= x + x_len; iloop++)
    {
        y_curf = tan(a_0) * (iloop - x - x_len) + y - y_len;
        y_curl = tan(a_144) * (iloop - x - x_len) + y - y_len;
        for (jloop = y_curf; jloop <= y_curl; jloop++)
        {
        	  fb_pixel(fb_inf, iloop, jloop, color);
        }
    }
    
    return 0;
}
