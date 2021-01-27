#ifndef _VTI_H
#define _VTI_H

void vti_set_start(unsigned int start);
void vti_print_at(int x, int y, unsigned char *msg);
void vti_center_at(int y, unsigned char *msg);
void vti_clear_screen(void);
void vti_rawchar_at(int x, int y, char ch);
void vti_plot(char mode, int x, int y);
void vti_line(char mode, int x0, int y0, int x1, int y1);
void vti_ellipse_rect(char mode, int x0, int y0, int x1, int y1);
void vti_box(char mode, int x0, int y0, int x1, int y1);
void vti_boxfill(char mode, int x0, int y0, int x1, int y1);
char vti_read_pixel(int x, int y);
unsigned char vti_read_char(int x, int y);
void vti_scroll_up(int n);
void vti_scroll_down(int n);
void vti_put_shape(int x, int y, char *shape, int w, int h);
void vti_set_keyboard_port(int port);
unsigned char vti_read_keyboard();
unsigned char vti_keypressed();

#endif
