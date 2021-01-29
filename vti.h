#ifndef _VTI_H
#define _VTI_H

#define VTI_WIDTH    64
#define VTI_HEIGHT   16
#define VTI_PAGESIZE (VTI_WIDTH * VTI_HEIGHT)

void vti_set_start(unsigned int start);
void vti_print_at(unsigned int x, unsigned int y, char *msg);
void vti_center_at(unsigned int y, char *msg);
void vti_clear_screen(void);
void vti_rawchar_at(unsigned int x, unsigned int y, char ch);
void vti_plot(unsigned char mode, unsigned int x, unsigned int y);
void vti_line(unsigned char mode, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void vti_ellipse_rect(unsigned char mode, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void vti_box(unsigned char mode, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void vti_boxfill(unsigned char mode, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
unsigned char vti_read_pixel(unsigned int x, unsigned int y);
unsigned char vti_read_char(unsigned int x, unsigned int y);
void vti_scroll_up(unsigned int n);
void vti_scroll_down(unsigned int n);
void vti_put_shape(unsigned int x, unsigned int y, char *shape, unsigned int w, unsigned int h);
void vti_set_keyboard_port(unsigned int port);
unsigned char vti_read_keyboard(void);
unsigned char vti_keypressed(void);

#endif
