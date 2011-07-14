/* mouse.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "frame.h"

#ifdef FRAME_SUPPORT_MOUSE

/////////////////////////////////////////////////////////////////////
#define C_WIDTH  10
#define C_HEIGHT 17
#define T___     0xFFFFFFFF
#define BORD     0x00000000
#define X___     0x0000FFFF
static unsigned long cursor_pixel[C_WIDTH * C_HEIGHT] = {
  BORD, T___, T___, T___, T___, T___, T___, T___, T___, T___,
  BORD, BORD, T___, T___, T___, T___, T___, T___, T___, T___,
  BORD, X___, BORD, T___, T___, T___, T___, T___, T___, T___,
  BORD, X___, X___, BORD, T___, T___, T___, T___, T___, T___,
  BORD, X___, X___, X___, BORD, T___, T___, T___, T___, T___,
  BORD, X___, X___, X___, X___, BORD, T___, T___, T___, T___,
  BORD, X___, X___, X___, X___, X___, BORD, T___, T___, T___,
  BORD, X___, X___, X___, X___, X___, X___, BORD, T___, T___,
  BORD, X___, X___, X___, X___, X___, X___, X___, BORD, T___,
  BORD, X___, X___, X___, X___, X___, X___, X___, X___, BORD,
  BORD, X___, X___, X___, X___, X___, BORD, BORD, BORD, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, T___, T___, T___,
  BORD, X___, BORD, T___, BORD, X___, X___, BORD, T___, T___,
  BORD, BORD, T___, T___, BORD, X___, X___, BORD, T___, T___,
  T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
  T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
  T___, T___, T___, T___, T___, T___, BORD, BORD, T___, T___
};
/*
static unsigned long cursor_pixel[C_WIDTH * C_HEIGHT] = {
  T___, T___, T___, T___, T___, T___, T___, T___, T___, T___,
  T___, T___, T___, BORD, BORD, BORD, BORD, T___, T___, T___,
  T___, T___, T___, BORD, X___, X___, BORD, T___, T___, T___,
  BORD, BORD, BORD, BORD, X___, X___, BORD, BORD, BORD, BORD,
  BORD, BORD, BORD, BORD, X___, X___, BORD, BORD, BORD, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, X___, X___, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, X___, X___, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, X___, X___, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, X___, X___, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, X___, X___, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, X___, X___, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, X___, X___, BORD,
  BORD, BORD, BORD, BORD, X___, X___, BORD, BORD, BORD, BORD,
  BORD, BORD, BORD, BORD, X___, X___, BORD, BORD, BORD, BORD,
  T___, T___, T___, BORD, X___, X___, BORD, T___, T___, T___,
  T___, T___, T___, BORD, BORD, BORD, BORD, T___, T___, T___,
  T___, T___, T___, T___, T___, T___, T___, T___, T___, T___
};*/
static unsigned long save_cursor[C_WIDTH * C_HEIGHT];

/////////////////////////////////////////////////////////////////////
/*
*  open mouse
*/
int mouse_open(char *device_name, int *fd)
{
    if (NULL == device_name)
		{
		    device_name = "/dev/input/mice";
		}
	  
	  if((*fd = open(device_name, O_RDONLY | O_NONBLOCK)) < 0)
	  {
		    fprintf(stderr, "Open %s:%s\n", device_name, strerror(errno));
		    return -1;
	  }
	
	  return 0;
}

/*
*  parse mouse move
*/
int mouse_parse(const u8_t *buf, mouse_event_t* mevent)
{
	    switch(buf[0] & 0x7)
	    {
	        case 1:						      /* left */
	        	    mevent->button = 1;
	        	break;
	        	
	        case 2:
	        	    mevent->button = 2;		/* right */
	        	break;
	        	
	        case 4:
	        	    mevent->button = 3;		/* middle */
	        	break;
	        	
	        default:
	        	break;
	    }
	    mevent->x =  buf[1] - ((buf[0] & 0x10) ? 256 : 0);
	    mevent->y = -buf[2] + ((buf[0] & 0x20) ? 256 : 0); 	    
	    return 0;
}

/*
*  get cursor color 
*/
static int fb_pixel_m(fb_info fb_inf, int x, int y, u32_t color)
{
    /* 16 bits */
    u8_t *pos = (u8_t*)fb_inf.fbmem + (fb_inf.w * y + x) * fb_inf.bpp/8;
    
    switch (fb_inf.bpp)
    {
        case 32:				              /* 32 bits */
        	*(pos + 3) = color >> 24;
        case 24:				              /* 24 bits */
        	*(pos + 2) = color >> 16;
        case 16:	  		              /* 16 bits */
        	*(pos + 1) = color >> 8;
        case 8:			                  /* 8 bits */
        	*pos = color;
        	return 0;
        	
        default:
        	return -1;
    }
    return -1;
}

/*
*  restore cursor
*/
int fb_restore_cursor(fb_info fb_inf, int x, int y)
{
    int iloop;
    int jloop;
    u32_t color;
    
    for (jloop = 0; jloop<C_HEIGHT; jloop++)
    {
        for (iloop = 0; iloop<C_WIDTH; iloop++)
        {
            color = save_cursor[iloop + jloop * C_WIDTH];
            fb_pixel_m(fb_inf, x+iloop, y+jloop, color);
        }
    }
    
    return 0;
}

/*
*  fb save cursor
*/
int fb_save_cursor (fb_info fb_inf,int x,int y)
{
    int iloop;
    int jloop;
    u32_t color;
    
    for (jloop = 0; jloop<C_HEIGHT; jloop++)
    {
        for (iloop = 0; iloop<C_WIDTH; iloop++)
        {
            color = *(u32_t*)(fb_inf.fbmem + ((y + jloop) * fb_inf.w + x + iloop) * fb_inf.bpp/8);
            save_cursor[iloop + jloop * C_WIDTH] = color;
        }
    }
    
	  return 0;
}

/*
*  fb draw cursor
*/
int fb_draw_cursor(fb_info fb_inf, int x, int y)
{
    int iloop;
    int jloop;
    u32_t color;
    
    for (jloop = 0; jloop<C_HEIGHT; jloop++)
    {
        for (iloop = 0; iloop<C_WIDTH; iloop++)
        {
			      color = cursor_pixel[iloop + jloop * C_WIDTH];
			      if (color != T___)
			      {
				        fb_pixel_m(fb_inf, x+iloop, y+jloop, color);
			      }
        }
    }
    
	  return 0;
}

#endif /* FRAME_SUPPORT_MOUSE */
