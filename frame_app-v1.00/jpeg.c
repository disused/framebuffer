/* jpeg.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <jpeglib.h>
#include <jerror.h>

#include "frame.h"

#ifdef FRAME_SUPPORT_JPEG
////////////////////////////////////////////////////////////////
/*
* jpeg error exit
*/
static void jpeg_error_exit(j_common_ptr cinfo)
{
    cinfo->err->output_message (cinfo);
    exit (EXIT_FAILURE);
}

/*
* jpeg decode
*/
unsigned char *decode_jpeg (const char *filename, fb_info *jpeg_inf)
{
    register JSAMPARRAY lineBuf;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr err_mgr;
    int bytesPerPix;
    FILE *inFile;
    unsigned char *retBuf;
    short w;
    short h;
    short *widthPtr = &w;
    short *heightPtr = &h;
    unsigned int col;
    int lineOffset = (*widthPtr * 3);
    int lineBufIndex;
    int xres ;
    int yres;			    
    int yloop;
    
    inFile = fopen (filename, "rb");
    if (NULL == inFile)
    { 
		printf ("Open file error %s\n",filename);
        return NULL;
    }
    
    cinfo.err = jpeg_std_error (&err_mgr);
    err_mgr.error_exit = jpeg_error_exit;	
    
    jpeg_create_decompress (&cinfo);
    jpeg_stdio_src (&cinfo, inFile);
    jpeg_read_header (&cinfo, 1);
    cinfo.do_fancy_upsampling = 0;
    cinfo.do_block_smoothing = 0;
    jpeg_start_decompress (&cinfo);
    
    jpeg_inf->w = cinfo.output_width;
    jpeg_inf->h = cinfo.output_height;
    
    w = jpeg_inf->w;
    h = jpeg_inf->h;
    
    bytesPerPix = cinfo.output_components;
    
    lineBuf = cinfo.mem->alloc_sarray ((j_common_ptr) &cinfo, JPOOL_IMAGE, (*widthPtr * bytesPerPix), 1);
    
    retBuf = (unsigned char *) malloc (3 * (*widthPtr * *heightPtr));
    if (NULL == retBuf)
    {
    	  return NULL;
    }
    
    if (3 == bytesPerPix)
    {
        for (yloop = 0; yloop < cinfo.output_height; ++yloop)
        {	
    		    jpeg_read_scanlines (&cinfo, lineBuf, 1);
    		    memcpy ((retBuf + (yloop * *widthPtr) * 3), lineBuf[0], 3 * *widthPtr);
    	  }
    }
    else if (1 == bytesPerPix)
    { 	
        for (yres = 0; yres < cinfo.output_height; ++yres) 
        {
        	   jpeg_read_scanlines (&cinfo, lineBuf, 1);
        	   	
        	   lineBufIndex = 0;
        	   for (xres = 0; xres < lineOffset; ++xres) 
        	   {
        	       col = lineBuf[0][lineBufIndex];
        	       
        	       retBuf[(lineOffset * yres) + xres] = col;
        	       ++xres;
        	       retBuf[(lineOffset * yres) + xres] = col;
        	       ++xres;
        	       retBuf[(lineOffset * yres) + xres] = col;
        	       ++lineBufIndex;
        	   }			
        }
    }
    else
    {
    	  return NULL;
    }
    jpeg_finish_decompress (&cinfo);
    jpeg_destroy_decompress (&cinfo);
    fclose (inFile);
    
    return retBuf;
}

/*
* rgb888 transfer to argb8888
*/
u32_t * rgb24to32(u8_t *buf24, fb_info jpeg_inf)
{
    int w = jpeg_inf.w;
    int h = jpeg_inf.h;
    int iloop;
    
    u32_t *buf = malloc(w * h * 4);
    if (buf == NULL)
    {
        return NULL;
    }
    
    for(iloop = 0; iloop < (w * h); ++iloop)
    {
        *((u8_t *)&buf[iloop] + 2) = buf24[iloop * 3 + 0];
        *((u8_t *)&buf[iloop] + 1) = buf24[iloop * 3 + 1];
        *((u8_t *)&buf[iloop] + 0) = buf24[iloop * 3 + 2];
        *((u8_t *)&buf[iloop] + 3) = 0x00;
    }
    return (u32_t *)buf;
}

/*
* scale24
*/
u8_t * scale24(u8_t *buf24, fb_info new_inf, fb_info jpeg_inf)
{
    int iloop;
    int jloop;
    int dtw;
    int dth;
    
    u8_t *buf = malloc(new_inf.w * new_inf.h * 3);
    
    if (buf == NULL)
    {
        return NULL;
    }
    
    for(jloop = 0; jloop < new_inf.h; jloop++)
    {
        for(iloop = 0; iloop < new_inf.w; iloop++)
        {
            dtw = (iloop * jpeg_inf.w) / new_inf.w;
            dth = (jloop * jpeg_inf.h) / new_inf.h;
            
            buf[(0 + (iloop * 3)) + (jloop * new_inf.w * 3)] = buf24[0 + ((dtw + (dth * jpeg_inf.w)) * 3)];
            buf[(1 + (iloop * 3)) + (jloop * new_inf.w * 3)] = buf24[1 + ((dtw + (dth * jpeg_inf.w)) * 3)];
            buf[(2 + (iloop * 3)) + (jloop * new_inf.w * 3)] = buf24[2 + ((dtw + (dth * jpeg_inf.w)) * 3)];
        }
    }
    
    return buf;
}

#endif    /* FRAME_SUPPORT_JPEG */
