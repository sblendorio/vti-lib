#ifndef _VTI_H
#define _VTI_H

#define VTI_WIDTH      64
#define VTI_HEIGHT     16
#define VTI_PAGESIZE   (VTI_WIDTH * VTI_HEIGHT)
#define VTI_BLANK_CHAR 0xa0

#define VTI_MODE_RESET  0
#define VTI_MODE_SET    1
#define VTI_MODE_INVERT 2
#define vti_setmode(x) (vti_mode = (x))
#define vti_getmode()  (vti_mode)

extern unsigned char vti_mode;

void vti_set_start(unsigned int start);
void vti_print_at(unsigned int x, unsigned int y, char *msg);
void vti_center_at(unsigned int y, char *msg);
void vti_clear_screen(void);
void vti_rawchar_at(unsigned int x, unsigned int y, char ch);
void vti_plot(unsigned int x, unsigned int y);
void vti_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void vti_ellipse_rect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void vti_box(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void vti_boxfill(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
unsigned char vti_read_pixel(unsigned int x, unsigned int y);
unsigned char vti_read_char(unsigned int x, unsigned int y);
void vti_scroll_up(unsigned int n);
void vti_scroll_down(unsigned int n);
void vti_put_shape(unsigned int x, unsigned int y, char *shape, unsigned int w, unsigned int h);

// VTI keyboard functions
void vti_set_keyboard_port(unsigned int port);
unsigned char vti_read_keyboard(void);
unsigned char vti_keypressed(void);
unsigned char vti_key_ascii(void);

#endif
