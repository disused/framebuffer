/* frame.h */
#ifndef _FRAME_H_
#define _FRAME_H_

#define FRAME_SUPPORT_JPEG
#define FRAME_SUPPORT_FONT
#define FRAME_SUPPORT_MOUSE

////////////////////////////////////////////////////////////////////////////
typedef unsigned char  u8_t;
typedef unsigned short u16_t;
typedef unsigned int   u32_t;

typedef struct 
{
    int w;				     /* width */
	  int h;				     /* high */
	  int bpp;				   /* bits per pixel */
	  u8_t *fbmem;
}fb_info;

////////////////////////////////////////////////////////////////////////////
/* initial framebuffer */
extern int init_fb(fb_info *fb_inf);

/* drawing pixel */
extern int fb_pixel(fb_info fb_inf, int x, int y, u32_t color);

/* painting horizontal */
extern int fb_pixel_row(fb_info fb_inf, int x, int y, int len, u32_t color);

/* draw a color ball */
extern int fb_draw_ball(const fb_info fb_inf, int x, int y, int len, u32_t color);

/* draw a color squarearea */
extern int fb_draw_squarearea(const fb_info fb_inf, int x, int y, int x_len, int y_len, u32_t color);

/* draw a triangle */
extern int fb_draw_triangle(const fb_info fb_inf, int x, int y, int len, u32_t color);

#ifdef FRAME_SUPPORT_FONT
////////////////////////////////////////////////////////////////////////////
/* init font library */
extern int init_ft (const char *file, int size);

/* display string */
extern int display_string (const char * buf, int x, int y, fb_info fb_inf, u32_t color);

#endif    /* FRAME_SUPPORT_FONT */

#ifdef FRAME_SUPPORT_MOUSE
////////////////////////////////////////////////////////////////////////////
typedef struct 
{
	int x;
	int y;
	int button;
}mouse_event_t;

////////////////////////////////////////////////////////////////////////////
/* open mouse */
extern int mouse_open(char *device_name, int *fd);
/* parse mouse */
extern int mouse_parse(const u8_t *buf, mouse_event_t* mevent);

/* restore cursor */
extern int fb_restore_cursor(fb_info fb_inf, int x, int y);
/* save cursor */
extern int fb_save_cursor (fb_info fb_inf,int x,int y);
/* draw cursor */
extern int fb_draw_cursor(fb_info fb_inf, int x, int y);

#endif  /* FRAME_SUPPORT_MOUSE */


#ifdef FRAME_SUPPORT_JPEG
////////////////////////////////////////////////////////////////////////////
/* decode jpeg */
extern unsigned char *decode_jpeg (const char *filename, fb_info *jpeg_inf);
/* rgb888 to argb8888 */
extern u32_t * rgb24to32(u8_t *buf24, fb_info jpeg_inf);
/* scale24 */
u8_t * scale24(u8_t *buf24, fb_info new_inf, fb_info jpeg_inf);

/* display jpeg */
extern int display_jpeg(char *filename, fb_info fb_inf);
extern int display_jpeg_move(char *filename, fb_info fb_inf);
extern int display_jpeg_shut(char *filename, fb_info fb_inf);
extern int display_jpeg_block(char *filename, fb_info fb_inf);
extern int display_jpeg_slopline(char *filename, fb_info fb_inf);
extern int display_jpeg_circle(char *filename, fb_info fb_inf);
extern int display_jpeg_squarearea(char *filename, fb_info fb_inf);
extern int display_jpeg_dot(char *filename, fb_info fb_inf);
extern int display_jpeg_chessboard(char *filename, fb_info fb_inf);
extern int display_jpeg_diamond(char *filename, fb_info fb_inf);
extern int display_jpeg_decr(char *filename, fb_info fb_inf,int x,int y,int len,int high);
extern int display_jpeg_transparent(char *filename1,char *filename2, fb_info fb_inf,float per1,float per2);
/* display jpeg */
extern int display_jpeg_shutter(char *filename, fb_info fb_inf);

extern int disp(char *filename, fb_info fb_inf);

#endif  /* FRAME_SUPPORT_JPEG */

/*display bmp*/
int disp_bmp(char *file_name,fb_info fb_inf);

int fb_draw_cir(const fb_info fb_inf, int x, int y, int len, u32_t color);
int fb_draw_ellipse(const fb_info fb_inf, int x, int y, int high, int width, u32_t color);
int fb_draw_upline(fb_info fb_inf, int x,int y, int len, u32_t color);
int fb_draw_rect(fb_info fb_inf, int x,int y, int high, int width, u32_t color);
int fb_draw_slopline(fb_info fb_inf, int x,int y, int len, u32_t color);

/*thread function*/
void* get_time(void* q);
void* test_keyboard(void* q);
void* play_picture(void* q);
#endif /* _FRAME_H_ */
