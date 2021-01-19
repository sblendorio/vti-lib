#include <vti.h>
#include <string.h>

unsigned char *vti_start = (unsigned char *) 0xf800;
static unsigned char pow[] = {1, 2, 4, 8, 16, 32};

unsigned char ascii2vti[] = {
  /*00*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  /*10*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  /*20*/ 0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
  /*30*/ 0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
  /*40*/ 0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
  /*50*/ 0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
  /*60*/ 0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
  /*70*/ 0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
  /*80*/ 0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
  /*90*/ 0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
  /*a0*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  /*b0*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  /*c0*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  /*d0*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  /*e0*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  /*f0*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

int absolute(int x);

int absolute(int x) {
    return x < 0 ? -x : x;
}

void vti_set_start(unsigned int start) {
    vti_start = (unsigned char *) start;
}

void vti_print_at(int x, int y, unsigned char *msg) {
	unsigned char *addr = vti_start + x + (64*y);
	while (*msg) *addr++ = ascii2vti[*msg++];
}

void vti_rawchar_at(int x, int y, char ch) {
	*(vti_start + x + (64*y)) = ch;
}

void vti_clear_screen(void) {
    memset(vti_start, 0xa0, 0x400);
}

void vti_plot(char mode, int x, int y) {
    static unsigned char *addr;
    static int gx, gy;
    static unsigned char sx, sy, mask, value;
    if (x > 127 || y > 47) return;
    gx = x / 2;
    gy = y / 3;
    sx = x % 2;
    sy = y % 3;
    mask = pow[5 - 3*sx - sy];
    addr = vti_start + gx + (64*gy);

    value = *addr;
    if (value & 0x80) value = 0x3f;

    *addr =
          mode == 1 ?
            value & (~mask)
        : mode == 0 ?
            value | mask
        : // mode = 2 -> XOR mode
            (~value & mask ? value | mask : value & (~mask));
}

void vti_box(char mode, int x0, int y0, int x1, int y1) {
    static int i;
    for (i=x0; i<=x1; ++i) {
        vti_plot(mode, i, y0);
        vti_plot(mode, i, y1);
    }
    for (i=y0; i<=y1; ++i) {
        vti_plot(mode, x0, i);
        vti_plot(mode, x1, i);
    }
}

void vti_boxfill(char mode, int x0, int y0, int x1, int y1) {
    static int i,j;
    for (i=x0; i<=x1; ++i)
        for (j=y0; j<=y1; ++j)
            vti_plot(mode, i, j);
}

// http://members.chello.at/~easyfilter/bresenham.html
void vti_line(char mode, int x0, int y0, int x1, int y1) {
   int dx =  absolute(x1-x0);
   int sx = x0<x1 ? 1 : -1;
   int dy = -absolute(y1-y0);
   int sy = y0<y1 ? 1 : -1; 
   int err = dx+dy, e2; /* error value e_xy */
 
   for(;;){  /* loop */
      vti_plot(mode,x0,y0);
      if (x0==x1 && y0==y1) break;
      e2 = 2*err;
      if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
      if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
   }
}

// http://members.chello.at/~easyfilter/bresenham.html
void vti_ellipse_rect(char mode, int x0, int y0, int x1, int y1)
{
   long a = absolute(x1-x0), b = absolute(y1-y0);
   long b1 = b&1; /* values of diameter */
   long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
   long err = dx+dy+b1*a*a, e2; /* error of 1.step */

   if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
   if (y0 > y1) y0 = y1; /* .. exchange them */
   y0 += (b+1)/2; y1 = y0-b1;   /* starting pixel */
   a *= 8*a; b1 = 8*b*b;

   do {
       vti_plot(mode, x1, y0); /*   I. Quadrant */
       vti_plot(mode, x0, y0); /*  II. Quadrant */
       vti_plot(mode, x0, y1); /* III. Quadrant */
       vti_plot(mode, x1, y1); /*  IV. Quadrant */
       e2 = 2*err;
       if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */ 
       if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; } /* x step */
   } while (x0 <= x1);
   
   while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
       vti_plot(mode, x0-1, y0); /* -> finish tip of ellipse */
       vti_plot(mode, x1+1, y0++); 
       vti_plot(mode, x0-1, y1);
       vti_plot(mode, x1+1, y1--); 
   }
}

char vti_read_pixel(int x, int y) {
    static unsigned char *addr;
    static int gx, gy;
    static unsigned char sx, sy, value;
    if (x > 127 || y > 47) return 0;
    gx = x / 2;
    gy = y / 3;
    sx = x % 2;
    sy = y % 3;
    
    value = *(vti_start + gx + (64*gy));
    if (value & 0x80) value = 0x3f;
    return ~value & pow[5 - 3*sx - sy] ? 1 : 0;
}

unsigned char vti_read_char(int x, int y) {
    return *(vti_start + x + (64*y));
}

void vti_scroll_up(int n) {
    memcpy(vti_start, vti_start + (0x40 * n), 0x0400 - (0x40 * n));
    memset(vti_start + 0x400 - (0x40 * n), 0xa0, 0x40*n);
}

void vti_scroll_down(int n) {
    memmove(vti_start + (0x40 * n), vti_start, 0x0400 - (0x40 * n));
    memset(vti_start, 0xa0, 0x40 * n);
}

void vti_put_shape(int x, int y, char **shape, int w, int h) {
    unsigned int ih, iw;
    char ch;
    for (ih = 0; ih < h; ++ih) {
        for (iw = 0; iw < w; ++iw) {
            ch = *(*(shape+ih)+iw);
            if (ch == '*' || ch =='.') vti_plot(ch == '*' ? 1 : 0, x+iw, y+ih);
        }
    }
}