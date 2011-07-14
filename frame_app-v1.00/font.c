/* font.c */
#include <ft2build.h>
#include <freetype/ftsizes.h>
#include FT_FREETYPE_H

#include "frame.h"

static FT_Library library;
static FT_Face face;

static char *fontfile = "mao.ttf";
static int psize = 32;

/*
*  utf8 transfer to unicode(utf32)
*/
static u16_t s_font_utf8_to_unicode (const unsigned char *utf8, int *len)
{
    unsigned short unicode;
    unicode = utf8[0];
    
    if (unicode >= 0xF0)
    {
        unicode = (unsigned short) (utf8[0] & 0x07) << 18;
        unicode |= (unsigned short) (utf8[1] & 0x3F) << 12;
        unicode |= (unsigned short) (utf8[2] & 0x3F) << 6;
        unicode |= (unsigned short) (utf8[3] & 0x3F);
        *len = 4;
    }
    else if (unicode >= 0xE0)
    {
        unicode = (unsigned short) (utf8[0] & 0x0F) << 12;
        unicode |= (unsigned short) (utf8[1] & 0x3F) << 6;
        unicode |= (unsigned short) (utf8[2] & 0x3F);
        *len = 3;
    }
    else if (unicode >= 0xC0)
    {
        unicode = (unsigned short) (utf8[0] & 0x1F) << 6;
        unicode |= (unsigned short) (utf8[1] & 0x3F);
        *len = 2;
    }
    else
    {
        *len = 1;
    }
    return unicode;
}

/*
*  init font library
*/
int init_ft (const char *file, int size)
{
    /* font size */
    if(size != 0)
    {
        psize = size;
    }
    
    /* font file */
    if (file != NULL)
    {
        fontfile = (char *)file;
    }
    
    /* load library */
    if ( FT_Init_FreeType (&library)
      || FT_New_Face (library, fontfile, 0, &face)
      || FT_Set_Pixel_Sizes (face, psize, 0))
    {
        printf("Error initial font file!!!\n");
    	  return 1;
    }
    
    return 0;
}

/*
*  display single font
*/
int display_font(const char *code, int *len, int x, int y, fb_info fb, u32_t color)
{
    unsigned int ucode;
    FT_UInt glyph_index;
    int row;
    int pixel;    

    ucode = s_font_utf8_to_unicode ((u8_t*)code, len);
    
    if ((glyph_index = FT_Get_Char_Index (face, ucode)) == 0)
    {
        return -1;
    }
    
    if (FT_Load_Glyph (face, glyph_index, FT_LOAD_DEFAULT))
    {	
        return -2;
    }
    
    if (FT_Render_Glyph (face->glyph, FT_RENDER_MODE_MONO))
    {
        return 4;
    }
    
	
	if(FT_Set_Char_Size(face,100,100,1024,768))
	{
		return -3;
	}

    y -= face->glyph->bitmap_top;
    
    for (row = 0; row < face->glyph->bitmap.rows; ++row, y++)
    {    
    	  for (pixel = 0; pixel < face->glyph->bitmap.width; ++pixel)
    	  {
    	      if (face->glyph->bitmap.buffer[row * face->glyph->bitmap.pitch + pixel / 8]
    	  	     & (0xC0 >> (pixel % 8)))
    	  	  {	
    	  		    fb_pixel(fb, x + pixel, y, color);
    	  		}
    	  }
    }
    
    return 0;
}

/*
*  display string (fonts)
*/
int display_string (const char * buf, int x, int y, fb_info fb_inf, u32_t color)
{
    int len;

	  while (buf[0] != '\0')
	  {	
		    if (display_font(buf, &len, x, y, fb_inf, color))
		    {
		        return -1;
		    }
		    buf += len;
		    x += psize;
	  }
    
	  return 0;
}
