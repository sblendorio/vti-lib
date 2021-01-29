#include <vti.h>
#include <string.h>

unsigned char *vti_start = (unsigned char *) 0xf800;
static unsigned char pow0[] = {32, 16, 8};
static unsigned char pow1[] = {4,  2,  1};

// precalculated table with address of row Y
static unsigned int vti_row[48] = {
    0,   0,   0,  64,  64,  64, 128, 128, 128,
  192, 192, 192, 256, 256, 256, 320, 320, 320,
  384, 384, 384, 448, 448, 448, 512, 512, 512,
  576, 576, 576, 640, 640, 640, 704, 704, 704,
  768, 768, 768, 832, 832, 832, 896, 896, 896,
  960, 960, 960
};

// precalculated table with Y % 3
static unsigned int vti_mod[48] = {
  0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1,
  2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0,
  1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2,
  0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1,
  2, 0, 1, 2
};

int absolute(int x);

int absolute(int x) {
    return x < 0 ? -x : x;
}

void vti_set_keyboard_port(unsigned int port) __z88dk_fastcall {
#asm
    ld a,l
    ld (portsmc+1),a
#endasm
}

unsigned char vti_read_keyboard(void) __z88dk_fastcall {
#asm
portsmc:
    in a,(0xf8)
    ld l,a
    ld h,0
#endasm
}

unsigned char vti_keypressed(void) {
    return vti_read_keyboard() & 0x80;
}

void vti_set_start(unsigned int start) {
    vti_start = (unsigned char *) start;
    vti_set_keyboard_port(start >> 8);
}

void vti_print_at(unsigned int x, unsigned int y, char *msg) {
	unsigned char *addr = vti_start + x + (VTI_WIDTH*y);
	while(*msg) {
		*addr++ = *((unsigned char *)msg)++ | 0x80;   // bit 7 on: ASCII TEXT
	}
}

void vti_center_at(unsigned int y, unsigned char *msg) {
    vti_print_at((VTI_WIDTH-strlen(msg))/2, y, msg);
}

void vti_rawchar_at(unsigned int x, unsigned int y, char ch) {
	*(vti_start + x + (VTI_WIDTH*y)) = ch;
}

void vti_clear_screen(void) {
    memset(vti_start, 0xa0, VTI_PAGESIZE);
}

void vti_plot(unsigned char mode, unsigned int x, unsigned int y) {
    static unsigned char *addr;
    static unsigned int gx;
    static unsigned char sx, sy, mask, value;

    if (x > 127 || y > 47) return;
    gx = x / 2;
    sx = x % 2;
    sy = vti_mod[y];
    mask = sx == 0 ? pow0[sy] : pow1[sy];
    addr = vti_start + gx + vti_row[y];

    value = *addr;
    if (value & 0x80) value = 0x3f;

    *addr =
          mode == 1 ?
            value & (~mask)
        : mode == 0 ?
            value | mask
        : // mode = 2 -> XOR mode
            value ^ mask;
}

unsigned char vti_read_pixel(unsigned int x, unsigned int y) {
    static unsigned int gx;
    static unsigned char sx, sy, mask, value;
    if (x > 127 || y > 47) return 0;
    gx = x / 2;
    sx = x % 2;
    sy = vti_mod[y];
    mask = sx == 0 ? pow0[sy] : pow1[sy];
    value = *(vti_start + gx + vti_row[y]);
    if (value & 0x80) value = 0x3f;
    return ~value & mask ? 1 : 0;
}

void vti_box(unsigned char mode, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
    static unsigned int i;
    for (i=x0; i<=x1; ++i) {
        vti_plot(mode, i, y0);
        vti_plot(mode, i, y1);
    }
    for (i=y0; i<=y1; ++i) {
        vti_plot(mode, x0, i);
        vti_plot(mode, x1, i);
    }
}

void vti_boxfill(unsigned char mode, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
    static unsigned int i,j;
    for (i=x0; i<=x1; ++i)
        for (j=y0; j<=y1; ++j)
            vti_plot(mode, i, j);
}

// http://members.chello.at/~easyfilter/bresenham.html
void vti_line(unsigned char mode, unsigned int _x0, unsigned int _y0, unsigned int _x1, unsigned int _y1) {

    static unsigned int x0,y0,x1,y1;
    static int dx,sx,dy,sy,err,e2;
    static unsigned char ix,iy;
    x0 = _x0;
    y0 = _y0;
    x1 = _x1;
    y1 = _y1;

    dx =  absolute(x1-x0);
    dy = -absolute(y1-y0);
    err = dx+dy; /* error value e_xy */

    ix = x0<x1;
    iy = y0<y1;

    for(;;){  /* loop */
        vti_plot(mode,x0,y0);
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 >= dy) { err += dy; if(ix) ++x0; else --x0; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; if(iy) ++y0; else --y0; } /* e_xy+e_y < 0 */
    }
}

// http://members.chello.at/~easyfilter/bresenham.html
void vti_ellipse_rect(unsigned char mode, unsigned int _x0, unsigned int _y0, unsigned int _x1, unsigned int _y1)
{
    static unsigned int x0,y0,x1,y1;
    x0 = _x0;
    y0 = _y0;
    x1 = _x1;
    y1 = _y1;

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

unsigned char vti_read_char(unsigned int x, unsigned int y) {
    return *(vti_start + x + (VTI_WIDTH*y));
}

void vti_scroll_up(unsigned int n) {
    memcpy(vti_start, vti_start + (VTI_WIDTH * n), VTI_PAGESIZE - (VTI_WIDTH * n));
    memset(vti_start + VTI_PAGESIZE - (VTI_WIDTH * n), 0xa0, VTI_WIDTH*n);
}

void vti_scroll_down(unsigned int n) {
    memmove(vti_start + (VTI_WIDTH * n), vti_start, VTI_PAGESIZE - (VTI_WIDTH * n));
    memset(vti_start, 0xa0, VTI_WIDTH * n);
}

void vti_put_shape(unsigned int x, unsigned int y, char *shape, unsigned int w, unsigned int h) {
    static unsigned int xx, yy, ex, ey;
    static char ch;
    ex = x+w;
    ey = y+h;
    for (yy = y; yy < ey; ++yy) {
        for (xx = x; xx < ex; ++xx) {
            ch = *shape++;
            vti_plot(ch == '*' ? 1 : 0, xx, yy);
        }
    }
}