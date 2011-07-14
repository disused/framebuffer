#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "frame.h"

int disp_bmp(char *file_name,fb_info fb_inf)
{
	FILE *fp = NULL;
	unsigned int file_size = 0;
	unsigned int offset = 0;
	int ret;
	int i,j;
	char *p = NULL;
	unsigned int high = 0;
	unsigned int width = 0;
	unsigned int colordepth = 0;
	char buf[100];

	fp = fopen(file_name,"r");
	if(fp == NULL)
	{
		printf("Open file failed\n");
		return -1;
	}
	ret = fread(buf,1,14,fp);
	/*printf("buf = ");
	for(i = 0;i<14; i++)
	{
		printf("%d, ",buf[i]);
	}
	printf("\n");*/
	
	/*calculate file size*/
	p = (char *)&file_size;
	*p = buf[2];
	*(p+1) = buf[3];
	*(p+2) = buf[4];
	*(p+3) = buf[5];	
	//printf("file size = %d\n",file_size);

	/*calculate offset*/
	p = (char *)&offset;
	*p = buf[10];
	*(p+1) = buf[11];
	*(p+2) = buf[12];
	*(p+3) = buf[13];	
	//printf("data offset = %d\n",offset);
		
	ret = fread(buf,1,40,fp);
	/*printf("buf = ");
	for(i = 0;i<40; i++)
	{
		printf("%d, ",buf[i]);
	}
	printf("\n");*/

	/*calculate high and width*/
	p = (char *)&width;
	*p = buf[4];
	*(p+1) = buf[5];
	*(p+2) = buf[6];
	*(p+3) = buf[7];

	p = (char *)&high;
	*p = buf[8];
	*(p+1) = buf[9];
	*(p+2) = buf[10];
	*(p+3) = buf[11];

	//printf("high = %d,width = %d\n",high,width);

	/*calculate color depth*/
	p = (char *)&colordepth;
	*p = buf[14];
	*(p+1) = buf[15];

	//printf("color depth = %d\n",colordepth);	

	/*disp bmp*/
	p = (char *)fb_inf.fbmem + fb_inf.w * fb_inf.h * fb_inf.bpp / 8 - width*4;

	for(i = 0;i < high;i++)
	{
		for(j = 0; j < width; j++)
		{
			ret = fread(buf,1,3,fp);
			*p = buf[0];
			*(p+1) = buf[1];
			*(p+2) = buf[2];
			p += 4;
		}
		p = p - (fb_inf.w + width)*4;
	}

	fclose(fp);
	return 0;
}
