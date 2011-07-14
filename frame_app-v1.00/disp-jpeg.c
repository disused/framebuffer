/* disp-jpeg.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include "frame.h"

#ifdef FRAME_SUPPORT_JPEG
////////////////////////////////////////////////////////////////
/*specially effect--shutter*/
int special_shut(fb_info fb_inf,u32_t *buf32)
{
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32;
	int i,j,k;

	for(k=0;k<153;k++)
	{
		for(i = 0; i<650 ; i+=153)
		{
			buf = buf32+(i+k)*fb_inf.w;
			p = (u32_t*)fb_inf.fbmem+(i+k)*fb_inf.w;
			for(j = 0; j<fb_inf.w; j++)	
			{
				*p = *buf;
				p++;
				buf++;
			}
			usleep(1000);
		}
	}
	
	buf = buf32 + (fb_inf.h-3)*fb_inf.w;
	p = (u32_t*)fb_inf.fbmem+(fb_inf.h-3)*fb_inf.w;
	for(j = 0; j<fb_inf.w*3; j++)	
	{
		*p = *buf;
		p++;
		buf++;
	}
	return 0;
}
/*specially effect--move from up to down*/
int special_move(fb_info fb_inf,u32_t *buf32)
{
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32 + 1024*767;
	int i,j;
	
	for(j = 0; j<767; j++)
	{
		p = (u32_t*)fb_inf.fbmem;
		
		for(i = 0; i<1024; i++)
		{
			*p = *buf;
			p++;
			buf++;	
		}
		memmove(fb_inf.fbmem+1024*4,fb_inf.fbmem,1024*4*(j+1));
		buf = buf - 2048;
		if(j<400)
			usleep(1000);
		else if(j>= 400 && j< 500)
			usleep(600);
		else if(j>= 500 && j< 600)
			usleep(300);
		else if(j>= 600 && j< 700)
			usleep(50);
		else if(j>= 700)
			;
	}

	return 0;
}

/*specially effect--dot*/
int print_16x16(fb_info fb_inf,u32_t *buf32,int n)
{
	int i,j;
	int x;
	int y;
	int row;
	int line;
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32;

	if(n<0)
		return -1;

	row = n%64;
	line = n/64;
	x = (line)*16;
	y = (row)*16;
	
	for(i=0;i<16;i++)
	{
		for(j=0;j<16;j++)
		{
			if((i+x)*1024+(j+y) > 1024*768 || (i+x)*1024+(j+y) < 0)
				continue;
			else
				*(p+(i+x)*1024+(j+y)) = *(buf+(i+x)*1024+(j+y));
		}
	}
	
	return 0;
}
int special_dot(fb_info fb_inf,u32_t *buf32)
{
	int i;
		
	/*for(i=1536,j=1537;i>0;i--,j++)
	{
		print_16x16(fb_inf,buf32,i);
		print_16x16(fb_inf,buf32,j);
		usleep(800);
	}
	print_16x16(fb_inf,buf32,0);*/

	for(i=0;i<3072;i+=3)
	{
		print_16x16(fb_inf,buf32,i);
		usleep(50);
	}
	//usleep(20000);
	for(i=3070;i>0;i-=3)
	{
		print_16x16(fb_inf,buf32,i);
		usleep(50);
	}
	//usleep(20000);
	for(i=2;i<3072;i+=3)
	{
		print_16x16(fb_inf,buf32,i);
		usleep(50);
	}
	return 0;
}

/*specially effect--block*/
int special_block(fb_info fb_inf,u32_t *buf32)
{
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32;
	int j,i;

	for(j = 0; j<768 ; j++)
	{	
		p = (u32_t*)fb_inf.fbmem+1024*j;
		buf = buf32+1024*j;
		for(i = 0; i<256; i++)
		{
			*p = *buf;
			p++;
			buf++;
		}
		usleep(300);
	}
	
	for(j = 767; j>=0 ; j--)
	{	
		p = (u32_t*)fb_inf.fbmem+1024*j+512;
		buf = buf32+1024*j+512;
		for(i = 0; i<257; i++)
		{
			*p = *buf;
			p--;
			buf--;
		}
		usleep(300);
	}
	for(j = 0; j<768 ; j++)
	{	
		p = (u32_t*)fb_inf.fbmem+1024*j+512;
		buf = buf32+1024*j+512;
		for(i = 0; i<256; i++)
		{
			*p = *buf;
			p++;
			buf++;
		}
		usleep(300);
	}
	
	for(j = 767; j>=0 ; j--)
	{	
		p = (u32_t*)fb_inf.fbmem+1024*j+1024;
		buf = buf32+1024*j+1024;
		for(i = 0; i<257; i++)
		{
			*p = *buf;
			p--;
			buf--;
		}
		usleep(300);
	}
	return 0;
}

/*specially effect--slopline*/
int special_slopline(fb_info fb_inf,u32_t *buf32)
{
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32;
	int i,j;


	/*for(i= 0;i<768;i++)
	{
		for(j=i; j<768;j++)
		{
			if(j*1024+j+i+1 > 1024*768)
				continue;
			else
			*(p + j*1024+j-i) = *(buf + j*1024+j-i);
		}
		usleep(500);
	}*/
	
	for(i = -513; i<512; i++)
	{
		for(j=0;j<768;j++)
		{
			if((j*1024+j+i+1) > 1024*768 || (j*1024+j+i+1)<0)
				continue;
			else
			*(p + j*1024+j+i+1) = *(buf + j*1024+j+i+1);
			if(( j*1024+j-i ) > 1024*768 || ( j*1024+j-i ) < 0)
				continue;
			else
			*(p + j*1024+j-i) = *(buf + j*1024+j-i);
		}
		usleep(500);
	}

	return 0;
}
/*specially effect--circle*/
int special_circle(fb_info fb_inf,u32_t *buf32)
{
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32;
	int iloop;
    	int jloop;
	int len;
	int x = 384;
	int y = 516;   

	for(len = 10; len<1024; len+=10)
	{
		for (iloop = x - len; iloop <= x + len; iloop++)
		{
			  for (jloop = y - len; jloop <= y + len; jloop++)
			  {
			  	  if ((iloop - x) * (iloop - x) + (jloop - y) * (jloop - y) <= len * len)
			  	  {
						if(iloop*1024+jloop > 1024*768 || iloop*1024+jloop < 0)
			  		    		continue;
						else
							*(p + iloop*1024+jloop) = *(buf + iloop*1024+jloop);
				  }
			  }
		}
		usleep(500);
	}

	return 0;
}

/*specially effect--squarearea*/
int special_squarearea(fb_info fb_inf,u32_t *buf32)
{
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32;
	int x_len;
	int y_len;	
	
	int iloop;
    	int jloop;
    
	for(x_len=0, y_len=0; x_len<=770||y_len<=1024; x_len += 10,y_len += 14)
	{
		for (jloop = 0; jloop < y_len; jloop++)
		{
		    for (iloop = 0; iloop < x_len; iloop++)
		    {
		        if(iloop*1024+jloop > 1024*768 || iloop*1024+jloop < 0)
					continue;
				else
					*(p + iloop*1024+jloop) = *(buf + iloop*1024+jloop);
		    }
		}
		//usleep(100);
	}
	return 0;
}
/*specially effect--chessboard*/
int special_chessboard(fb_info fb_inf,u32_t *buf32)
{
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32;
	int i,j,k,m,n,t;
	j=0;

	for(i = 0; i <= fb_inf.w/20; i++)
	{
                for(t=0;t<10;t++)
			for (j = 0; j <= fb_inf.h/20; j++)
			{
		             for(k=0;k<10;k++)
		             {
				m=(2*k+1)*fb_inf.w/20+i;
		                 n=2*fb_inf.h*t/20+j;
				*(p+m+n*fb_inf.w) = *(buf +m+n*fb_inf.w);
		                  m=2*k*fb_inf.w/20+i;
		                  n=(2*t+1)*fb_inf.h/20+j;
		               
				 *(p+m+n*fb_inf.w) = *(buf +m+n*fb_inf.w);
		                
		             }

			}
      		usleep(1000);    
	}
	for(i = fb_inf.w/20; i >=0 ; i--)
	{
                for(t=0;t<10;t++)
			for (j = 0; j <= fb_inf.h/20; j++)
			{
		             for(k=0;k<10;k++)
		             {   
				n=(2*t+1)*fb_inf.h/20+j;
		                 m=(2*k+1)*fb_inf.w/20+i;
		              
				*(p+m+n*fb_inf.w) = *(buf +m+n*fb_inf.w);
		                  m=2*fb_inf.w*k/20+i;
		               
		                 n=2*t*fb_inf.h/20+j;
				*(p+m+n*fb_inf.w) = *(buf +m+n*fb_inf.w);
		                 
		             }
			}
       		usleep(1000);    
	}
	return 0;
}

/*specially effect--diamond*/
int special_diamond(fb_info fb_inf,u32_t *buf32)
{
	u32_t *p = (u32_t*)fb_inf.fbmem;
	u32_t *buf = buf32;
	
	int R,r,j,i;
         R=sqrt(fb_inf.w*fb_inf.w/4+fb_inf.h*fb_inf.h/4) ;
        for(r=0;r<fb_inf.w;r+=20)
           { 
                      
	     
		for (j = 0; j < fb_inf.h; ++j){
                   for(i = 0; i < fb_inf.w; ++i){
                     
                     if(fabs(j-fb_inf.h/2)+fabs(i-fb_inf.w/2)<=r)
                       
			*(p +i+j*fb_inf.w) = *(buf +i+j*fb_inf.w);
                     }
                    
		}
                            usleep(100);    
	}

	return 0;
}

/*
* display jpeg
*/
int display_jpeg_move(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_move(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_shut(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_shut(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_block(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_block(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_slopline(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_slopline(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_circle(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_circle(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_squarearea(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_squarearea(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_dot(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_dot(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_chessboard(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_chessboard(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_diamond(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;
    
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
    
    special_diamond(fb_inf,buf32);
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_decr(char *filename, fb_info fb_inf,int x,int y,int len,int high)
{
    fb_info jpeg_inf;
    //int xres;
    //int yres;

	fb_inf.w = len;
	fb_inf.h = high;
	

    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
  
	int i=0;
	int j=0;
	u32_t *p = (u32_t*)fb_inf.fbmem + y*1024 + x;
	u32_t *buf = buf32;

    for(i = 0; i<=high; i++)
	{
		for(j = 0; j<=len; j++)
		{
			*(p + (i)*1024 + j) = *(buf + (i)*len + j);
		}

	}
    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    int xres;
    int yres;



    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
 
 
    for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    
    return 0;
}
int display_jpeg_transparent(char *filename1,char *filename2, fb_info fb_inf,float per1,float per2)
{
    fb_info jpeg_inf;
    fb_info jpeg_inf1;

    //int xres;
    //int yres;

    u8_t *buf24 = decode_jpeg(filename1, &jpeg_inf);
    u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

    u8_t *buf241 = decode_jpeg(filename2, &jpeg_inf1);
    u8_t *scale_buf1 = scale24(buf241, fb_inf, jpeg_inf1);
    u32_t *buf321 = rgb24to32(scale_buf1, fb_inf);

	int i=0;
	int j=0;
	u8_t *p = (u8_t*)fb_inf.fbmem ;
	u8_t *buf = (u8_t *)buf32;
	u8_t *buf1 = (u8_t *)buf321;

    	for(i = 0; i<fb_inf.h*fb_inf.bpp / 8; i++)
	{
		for(j = 0; j<fb_inf.w; j++)
		{
			*(p + i*fb_inf.w +j ) = (*(buf + i*fb_inf.w +j )) * per1 + (*(buf1 + i*fb_inf.w +j )) * per2;
		}

	}

    /*for(xres = 0; xres < fb_inf.h; ++xres)
    {
        for (yres = 0; yres < fb_inf.w; ++yres)
        {
    		    fb_pixel(fb_inf, yres, xres, buf32[yres + (xres * fb_inf.w)]);
        }
    }*/
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    free(buf241);
    free(scale_buf1);
    free(buf321);
    
    return 0;
}

#endif    /* FRAME_SUPPORT_JPEG */
