#include <sys/mman.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "frame.h"

char save_buf[150000];

static int save_frame(fb_info fb_inf,int x,int y,int len,int high)
{
	int i;
	int j;
	char* p;
	int q=0;
	
	
	for(j=y; j <= y+high; j++)
	{
		p = (char *)fb_inf.fbmem + (j*fb_inf.w + x)*fb_inf.bpp / 8;
		for(i=x; i<= x+len*4; i++)
		{
			save_buf[q] = *p;
			p++;
			q++;
		}
	}
	return 0;
}

static int load_frame(fb_info fb_inf,int x,int y,int len,int high)
{
	int i;
	int j;
	char* p;
	int q=0;
	
	
	for(j=y; j <= y+high; j++)
	{
		p = (char *)fb_inf.fbmem + (j*fb_inf.w + x)*fb_inf.bpp / 8;
		for(i=x; i<= x+len*4; i++)
		{
			*p=save_buf[q];
			p++;
			q++;
		}
	}
	return 0;
}

void* get_time(void* q)
{
	struct tm *local_time = NULL;
	time_t timer;
	char *p = NULL;
	fb_info fb_inf;
    	char time_buf[30];
	char week_buf[10];
	char mon_buf[10];
	int hour;
	int min;
	int sec;
	int year;
	//int mon;
	int day;	
	
	if (init_fb(&fb_inf) < 0)
	{
	        fprintf(stderr, "Error initial framebuffer\b");
		return NULL;
	}
#ifdef FRAME_SUPPORT_FONT
   	if (init_ft("simsun.ttc", 10) < 0)
    	{
    	  	fprintf(stderr, "Error initial font\b");
    		  return NULL;
    	}
#endif

	sleep(1);
	fb_draw_rect(fb_inf, 775,75, 50, 229, 0x0000FF00);
	save_frame(fb_inf,775,75,230,50);
	while(1)
	{
		timer = time(NULL);
		local_time = localtime(&timer);
		p = asctime(local_time);
		//printf("%s\n",p);
		memset(week_buf,0,10);
		memset(time_buf,0,30);
		memset(mon_buf,0,30);
		sscanf(p,"%c%c%c %c%c%c %d %d:%d:%d %d",&week_buf[0],&week_buf[1],&week_buf[2],&mon_buf[0],&mon_buf[1],&mon_buf[2],&day,&hour,&min,&sec,&year);
		
		sprintf(time_buf,"%d-%s-%d %d:%d:%d",year,mon_buf,day,hour,min,sec);
		
		load_frame(fb_inf,775,75,230,50);
		display_string (time_buf, 795, 95, fb_inf,0x00ffffff);
		display_string (week_buf, 795, 120, fb_inf,0x00ffffff);
		usleep(20000);
	}

	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);
	return NULL;
}
