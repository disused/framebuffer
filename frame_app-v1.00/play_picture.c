#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#include "frame.h"


extern int flag_play;
extern pthread_mutex_t play_mut;

static int is_play(void)
{
	pthread_mutex_lock(&play_mut);
	if(flag_play == 1)
	{
		pthread_mutex_unlock(&play_mut);
		return 1;
	}
	else if(flag_play == 0)
	{
		pthread_mutex_unlock(&play_mut);
		return 0;
	}
	return -1;
}
void* play_picture(void* q)
{
	fb_info fb_inf;
	
	fb_inf.w = ((fb_info *)q)->w;
	fb_inf.h = ((fb_info *)q)->h;
	fb_inf.bpp = ((fb_info *)q)->bpp;
	fb_inf.fbmem = ((fb_info *)q)->fbmem;

	while(1)
	{
		display_jpeg_circle("1024X768_1.jpg", fb_inf);
		sleep(1);
    		while(!is_play());
    		display_jpeg_chessboard("GQ1.jpg", fb_inf);
		sleep(1);
    		while(!is_play());
		display_jpeg_shut("1024X768_1.jpg", fb_inf);
		sleep(1);
    		while(!is_play());
		display_jpeg_diamond("GQ.jpg", fb_inf);
		sleep(1);
    		while(!is_play());
		display_jpeg_move("1024X768_2.jpg", fb_inf);
		sleep(1);
    		while(!is_play());
		display_jpeg_block("1024X768_1.jpg", fb_inf);
		sleep(1);
    		while(!is_play());
		display_jpeg_slopline("1024X768_2.jpg", fb_inf);
		sleep(1);
    		while(!is_play());
	}
}
