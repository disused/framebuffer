/* main.c */
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#include "frame.h"

#ifdef FRAME_SUPPORT_MOUSE

int m_x;
int m_y;
int left_button=0;
int right_button=0;

extern int flag_play;
extern pthread_mutex_t play_mut;

pthread_mutex_t button_mut;

/*
* mouse test
*/
void* test_mouse(void* q)
{	
	sleep(1);
	fb_info fb_inf;

	fb_inf.w = ((fb_info *)q)->w;
	fb_inf.h = ((fb_info *)q)->h;
	fb_inf.bpp = ((fb_info *)q)->bpp;
	fb_inf.fbmem = ((fb_info *)q)->fbmem;
	/*if (init_fb(&fb_inf) < 0)
    {
    	 fprintf(stderr, "Error initial framebuffer\b");
    	 return NULL;
    }*/

    int mfd = -1;

    u8_t buf[8];
    mouse_event_t mevent;

    m_x = fb_inf.w / 2;
    m_y = fb_inf.h / 2;
        
    mouse_open(NULL, &mfd);
   
    fb_save_cursor (fb_inf,m_x, m_y);   
    fb_draw_cursor(fb_inf, m_x, m_y);
   
    for(;;)
     {
        if (read(mfd, buf, 8) != -1)
         {
			 mouse_parse(buf,&mevent);
			 //printf("dx:%d\tdy:%d\n", mevent.x, mevent.y);
			 //printf("mx:%d\tmy:%d\n", m_x, m_y);

		        //if((m_y > 8) && ((m_y < (768 - 8))))
		          {
			     fb_restore_cursor(fb_inf, m_x, m_y);
		          }
			 m_x += mevent.x;
			 m_y += mevent.y;
			 
		     if(m_y < 0)
				m_y = 0;
			 if(m_y >= 751)
				m_y = 751;
	 	     if(m_x < 0)
				m_x = 0;
			 if(m_x >= 1014)
				m_x = 1014;

			 fb_save_cursor (fb_inf,m_x, m_y);

		        //if((m_y > 8) && ((m_y < (768 - 8))))
		          {
			     fb_draw_cursor(fb_inf, m_x, m_y);
		          }

			 switch (mevent.button)
		     {
					case 1:
						
				  		//printf("left\n");
						pthread_mutex_lock(&button_mut);
						left_button++;
						pthread_mutex_unlock(&button_mut);
						mevent.button = 0;
						break;

			  		case 2:
						right_button++;
				 		// printf("right\n");
			   	 		break;
		                 
			  		case 3:
				 		// printf("middle\n");
						break;

			 		default:
				 		break;
			 }
	 	}	
        usleep(500);
     }
     
    return NULL;
}

#endif    /* FRAME_SUPPORT_MOUSE */

/* main */
int main(int argc, char *argv[]) 
{
	fb_info fb_inf;
	
	//pthread_t time_pid;
	//pthread_create(&time_pid,NULL,get_time,NULL);
	
	if (init_fb(&fb_inf) < 0)
	{
		fprintf(stderr, "Error initial framebuffer\b");
		return -1;
	}
	
	//pthread_t mouse_pid;
	//pthread_create(&mouse_pid,NULL,test_mouse,(void *)&fb_inf);
	//pthread_mutex_init(&button_mut,NULL);

	pthread_mutex_init(&play_mut,NULL);
	pthread_t keyboard_pid;
	pthread_create(&keyboard_pid,NULL,test_keyboard,NULL);

	pthread_t play_pict_pid;
	pthread_create(&play_pict_pid,NULL,play_picture,(void *)&fb_inf);
	pthread_join(play_pict_pid,NULL);
#ifdef FRAME_SUPPORT_JPEG
#if 0
		display_jpeg_transparent("GQ5.jpg","1024X768_1.jpg",fb_inf,0.3,0.7);
		while(1);
		sleep(2);
start:	
		
		display_jpeg("GQ5.jpg", fb_inf);
    		sleep(1);
		display_jpeg_decr("eggs.jpg", fb_inf,100,100,200,150);
		display_jpeg_decr("1024X768_2.jpg", fb_inf,350,100,200,150);
	
		display_jpeg_decr("1024X768_3.jpg", fb_inf,600,100,200,150);
	
		display_jpeg_decr("1024X768_1.jpg", fb_inf,100,450,200,150);
	
		display_jpeg_decr("GQ.jpg", fb_inf,350,450,200,150);
	
		display_jpeg_decr("GQ1.jpg", fb_inf,600,450,200,150);
	
	while(1)
	{
		if(m_x >= 100 &&  m_x <= 300 && m_y >= 100 && m_y <= 250 ) 
		{
			if(left_button == 2)//button action
			{
				display_jpeg("eggs.jpg", fb_inf);
				left_button = 0;
				pthread_mutex_lock(&button_mut);
				while(left_button != 2)
				{
					pthread_mutex_unlock(&button_mut);
					usleep(800);
					pthread_mutex_lock(&button_mut);
				}
				pthread_mutex_unlock(&button_mut);
				if(left_button ==2)
				{
					left_button = 0;
					goto start;
				}
			}
			else
			{
				if(m_x >= 100 &&  m_x <= 300 && m_y >= 100 && m_y <= 250 )
				{
					display_jpeg_decr("eggs.jpg", fb_inf,100,100,300,250);
				}
				
				while(m_x >= 100 &&  m_x <= 300 && m_y >= 100 && m_y <= 250)
				{
					if(left_button == 2)
					{
						display_jpeg("eggs.jpg", fb_inf);
						left_button = 0;
						pthread_mutex_lock(&button_mut);
						while(left_button != 2)
						{
							pthread_mutex_unlock(&button_mut);
							usleep(800);
							pthread_mutex_lock(&button_mut);
						}
						pthread_mutex_unlock(&button_mut);
						if(left_button ==2)
						{
							left_button = 0;
							goto start;
						}
					}
				}
				
				goto start;
			}
		}
		else
		if(m_x >= 350 &&  m_x <= 550 && m_y >= 100 && m_y <= 250)
		{
			if(left_button == 2)
			{
				display_jpeg("1024X768_2.jpg", fb_inf);
				left_button = 0;
				pthread_mutex_lock(&button_mut);
				while(left_button != 2)
				{
					pthread_mutex_unlock(&button_mut);
					usleep(800);
					pthread_mutex_lock(&button_mut);
				}
				pthread_mutex_unlock(&button_mut);
				if(left_button ==2)
				{
					left_button = 0;
					goto start;
				}
			}
			else
			{
				if(m_x >= 350 &&  m_x <= 550 && m_y >= 100 && m_y <= 250)
				{
					display_jpeg_decr("1024X768_2.jpg", fb_inf,350,100,300,250);
				}
				
				while(m_x >= 350 &&  m_x <= 550 && m_y >= 100 && m_y <= 250)
				{
					if(left_button == 2)
					{
						display_jpeg("1024X768_2.jpg", fb_inf);
						left_button = 0;
						pthread_mutex_lock(&button_mut);
						while(left_button != 2)
						{
							pthread_mutex_unlock(&button_mut);
							usleep(800);
							pthread_mutex_lock(&button_mut);
						}
						pthread_mutex_unlock(&button_mut);
						if(left_button ==2)
						{
							left_button = 0;
							goto start;
						}
					}
				}
				
				goto start;
			}
		}
		else
		if(m_x >= 600 &&  m_x <= 800 && m_y >= 100 && m_y <= 250 )
		{
			if(left_button == 2)
			{
				display_jpeg("1024X768_3.jpg", fb_inf);
				left_button = 0;
				pthread_mutex_lock(&button_mut);
				while(left_button != 2)
				{
					pthread_mutex_unlock(&button_mut);
					usleep(800);
					pthread_mutex_lock(&button_mut);
				}
				pthread_mutex_unlock(&button_mut);
				if(left_button ==2)
				{
					left_button = 0;
					goto start;
				}
			}
			else
			{
				if(m_x >= 600 &&  m_x <= 800 && m_y >= 100 && m_y <= 250)
				{
					display_jpeg_decr("1024X768_3.jpg", fb_inf,600,100,300,250);
				}
				
				while(m_x >= 600 &&  m_x <= 800 && m_y >= 100 && m_y <= 250)
				{
					if(left_button == 2)
					{
						display_jpeg("1024X768_3.jpg", fb_inf);
						left_button = 0;
						pthread_mutex_lock(&button_mut);
						while(left_button != 2)
						{
							pthread_mutex_unlock(&button_mut);
							usleep(800);
							pthread_mutex_lock(&button_mut);
						}
						pthread_mutex_unlock(&button_mut);
						if(left_button ==2)
						{
							left_button = 0;
							goto start;
						}
					}
				}
				
				goto start;
			}
		}
		else
		if(m_x >= 100 &&  m_x <= 300 && m_y >= 450 && m_y <= 600)
		{
			if(left_button == 2)
			{
				display_jpeg("1024X768_1.jpg", fb_inf);
				left_button = 0;
				pthread_mutex_lock(&button_mut);
				while(left_button != 2)
				{
					pthread_mutex_unlock(&button_mut);
					usleep(800);
					pthread_mutex_lock(&button_mut);
				}
				pthread_mutex_unlock(&button_mut);
				if(left_button ==2)
				{
					left_button = 0;
					goto start;
				}
			}
			else
			{
				if(m_x >= 100 &&  m_x <= 300 && m_y >= 450 && m_y <= 600)
				{
					display_jpeg_decr("1024X768_1.jpg", fb_inf,100,450,300,250);
				}
				
				while(m_x >= 100 &&  m_x <= 300 && m_y >= 450 && m_y <= 600)
				{
					if(left_button == 2)
					{
						display_jpeg("1024X768_1.jpg", fb_inf);
						left_button = 0;
						pthread_mutex_lock(&button_mut);
						while(left_button != 2)
						{
							pthread_mutex_unlock(&button_mut);
							usleep(800);
							pthread_mutex_lock(&button_mut);
						}
						pthread_mutex_unlock(&button_mut);
						if(left_button ==2)
						{
							left_button = 0;
							goto start;
						}
					}
				}
				
				goto start;
			}
			
		}
		else
		if(m_x >= 350 &&  m_x <= 550 && m_y >= 450 && m_y <= 600)
		{
			if( left_button == 2 )
			{
				display_jpeg("GQ.jpg", fb_inf);
				left_button = 0;
				pthread_mutex_lock(&button_mut);
				while(left_button != 2)
				{
					pthread_mutex_unlock(&button_mut);
					usleep(800);
					pthread_mutex_lock(&button_mut);
				}
				pthread_mutex_unlock(&button_mut);
				if(left_button ==2)
				{
					left_button = 0;
					goto start;
				}
			}
			else
			{
				if(m_x >= 350 &&  m_x <= 550 && m_y >= 450 && m_y <= 600)
				{
					display_jpeg_decr("GQ.jpg", fb_inf,350,450,300,250);
				}
				
				while(m_x >= 350 &&  m_x <= 550 && m_y >= 450 && m_y <= 600)
				{
					if(left_button == 2)
					{
						display_jpeg("GQ.jpg", fb_inf);
						left_button = 0;
						pthread_mutex_lock(&button_mut);
						while(left_button != 2)
						{
							pthread_mutex_unlock(&button_mut);
							usleep(800);
							pthread_mutex_lock(&button_mut);
						}
						pthread_mutex_unlock(&button_mut);
						if(left_button ==2)
						{
							left_button = 0;
							goto start;
						}
					}
				}
				
				goto start;
			}
			
		}
		else
		if(m_x >= 600 &&  m_x <= 800 && m_y >= 450 && m_y <= 600)
		{
			if( left_button == 2 )
			{
				display_jpeg("GQ1.jpg", fb_inf);
				left_button = 0;
				pthread_mutex_lock(&button_mut);
				while(left_button != 2)
				{
					pthread_mutex_unlock(&button_mut);
					usleep(800);
					pthread_mutex_lock(&button_mut);
				}
				pthread_mutex_unlock(&button_mut);
				if(left_button ==2)
				{
					left_button = 0;
					goto start;
				}
			}
			else
			{
				if(m_x >= 600 &&  m_x <= 800 && m_y >= 450 && m_y <= 600)
				{
					display_jpeg_decr("GQ1.jpg", fb_inf,600,450,300,250);
				}
				
				while(m_x >= 600 &&  m_x <= 800 && m_y >= 450 && m_y <= 600)
				{
					if(left_button == 2)
					{
						display_jpeg("GQ1.jpg", fb_inf);
						left_button = 0;
						pthread_mutex_lock(&button_mut);
						while(left_button != 2)
						{
							pthread_mutex_unlock(&button_mut);
							usleep(800);
							pthread_mutex_lock(&button_mut);
						}
						pthread_mutex_unlock(&button_mut);
						if(left_button ==2)
						{
							left_button = 0;
							goto start;
						}
					}
				}
				
				goto start;
			}
			
		}
		else
		if(left_button >= 2)
		{
				left_button = 0;
		}
		
		
	}
#endif
#endif
#if 0
    /* draw a squarearea */
    fb_draw_squarearea(fb_inf, 0, 0, fb_inf.w, fb_inf.h, 0xFF0000);
    
    /* draw five triangle */    
    fb_draw_triangle(fb_inf, 200, 220, 150, 0x00FFFF00);
    
    fb_draw_triangle(fb_inf, 380, 70, 40, 0x00FFFF00);	
    fb_draw_triangle(fb_inf, 440, 170, 40, 0x00FFFF00);
    fb_draw_triangle(fb_inf, 430, 270, 40, 0x00FFFF00);
    fb_draw_triangle(fb_inf, 380, 370, 40, 0x00FFFF00);
	//fb_draw_row(fb_inf,775,125,225,0x0000FF00);
	//fb_draw_row(fb_inf,775,75,225,0x0000FF00);
	//fb_draw_upline(fb_inf, 775,75, 50, 0x0000FF00);
	//fb_draw_upline(fb_inf, 1000,75,50, 0x0000FF00);
#endif
	
#ifdef FRAME_SUPPORT_FONT
    if (init_ft("simsun.ttc", 10) < 0)
    {
    	  fprintf(stderr, "Error initial font\b");
    	  return -1;
    }
    
    //display_string ("北京 中关村", 10, fb_inf.h - 100, fb_inf,0x00FFFF00);
#endif
    
#ifdef FRAME_SUPPORT_MOUSE
    //test_mouse(fb_inf);
#endif

    munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);
    
    return 0;
}
