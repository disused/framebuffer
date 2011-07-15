#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <pthread.h>

int flag_play=1;
pthread_mutex_t play_mut;

void handler (int sig)
{
  printf ("\nexiting...(%d)\n", sig);
  exit (0);
}
 
void perror_exit (char *error)
{
  perror (error);
  handler (9);
}
 
void* test_keyboard(void* q)
{
	  struct input_event ev[64];
	  int fd, rd, value, size = sizeof (struct input_event);
	  char *device = "/dev/input/event3";

	  //Open Device
	  if ((fd = open (device, O_RDONLY)) == -1)
	    printf ("%s is not a vaild device.\n", device);
	 
	 
	  while (1)
	  {
	      if ((rd = read (fd, ev, size * 64)) < size)
		  perror_exit ("read()");     
	 
	      value = ev[0].value;
	 
	      if (value != ' ' && ev[1].value == 1 && ev[1].type == 1){ // Only read the key press event
	      		//printf ("Code1[%d]\n", (ev[1].code));
			if(ev[1].code == 31)
			{
				pthread_mutex_lock(&play_mut);
				flag_play = 1;
				pthread_mutex_unlock(&play_mut);
			}
			else if(ev[1].code == 18)
			{
				pthread_mutex_lock(&play_mut);
				flag_play = 0;
				pthread_mutex_unlock(&play_mut);
			}
	      }
	  }
	 
	  return 0;
}
