#include <vti.h>
#include <string.h>

unsigned char *vti_start = (unsigned char *) 0xf800;
unsigned char vti_mode = VTI_MODE_SET;
unsigned char vti_pow0[] = {32, 16, 8};
unsigned char vti_pow1[] = {4,  2,  1};
unsigned char vti_pow[] = {32, 16, 8, 0,  4,  2,  1, 0};

// precalculated table with address of row Y
unsigned int vti_row[48] = {
    0,   0,   0,  64,  64,  64, 128, 128, 128,
  192, 192, 192, 256, 256, 256, 320, 320, 320,
  384, 384, 384, 448, 448, 448, 512, 512, 512,
  576, 576, 576, 640, 640, 640, 704, 704, 704,
  768, 768, 768, 832, 832, 832, 896, 896, 896,
  960, 960, 960
};

// precalculated table with Y % 3
unsigned char vti_mod[48] = {
  0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1,
  2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0,
  1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2,
  0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1,
  2, 0, 1, 2
};

int vti_abs(int x);

int vti_abs(int x) {
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
		*addr++ = *(unsigned char *)msg++ | 0x80;   // bit 7 on: ASCII TEXT
	}
}

void vti_center_at(unsigned int y, char *msg) {
    vti_print_at((VTI_WIDTH-strlen(msg))/2, y, msg);
}

void vti_rawchar_at(unsigned int x, unsigned int y, char ch) {
	*(vti_start + x + (VTI_WIDTH*y)) = ch;
}

void vti_clear_screen(void) {
    memset(vti_start, VTI_BLANK_CHAR, VTI_PAGESIZE);
}

void vti_plot(unsigned int x, unsigned int y) {
    static unsigned char *addr;
    static unsigned char xx, yy;
    // static unsigned char sx, sy, mask, value, gx;

    /*
    // the original routine in C:
    xx = (unsigned char) x;
    yy = (unsigned char) y;

    if (xx > 127) return;
    sx = (xx & 1)*4;
    gx = xx >> 1;

    if (yy > 47) return;
    sy = vti_mod[yy];
    mask = vti_pow[sx+sy];

    addr = vti_start + gx + vti_row[yy];

    value = *addr;
    if (value & 0x80) value = 0x3f;
         if(vti_mode == VTI_MODE_RESET) { *addr = value |  mask; return; }
    else if(vti_mode == VTI_MODE_SET)   { *addr = value & ~mask; return; }
    else                                { *addr = value ^  mask; return; }
    */

    xx = (unsigned char) x;
    yy = (unsigned char) y;

    // if (xx > 127) return;
    __asm
        ld   a, (_st_vti_plot_xx)
        ld   e, a    ; save xx into E
        and  $80
        ret  nz
    __endasm;

    // sx = (xx & 1)*4;
    __asm
        ld   a,e     ; gets xx from E
        and  $01
        rla
        rla
        ld   b,a     ; save sx into B
    __endasm;

    // gx = xx >> 1;
    __asm
        ld   a,e     ; gets xx from E
        rra
        ld   c,a     ; save gx into C
    __endasm;

    // if (yy > 47) return;
    __asm
        ld  a,(_st_vti_plot_yy)
        cp  47
        ret nc
        ld  e, a                   ; save yy into E
    __endasm;

    // sy = vti_mod[yy];
    __asm
        ld h, _vti_mod / 256
        ld l, _vti_mod % 256       ; HL = vti_mod
        ld d, 0                    ; DE = yy
        add hl, de
        ld a, (hl)
        ld e, a                    ; save sy into E
    __endasm;

    //mask = vti_pow[sx+sy];
    __asm
        ld h, _vti_pow / 256
        ld l, _vti_pow % 256      ; HL = vti_pow

        ld  a,b                   ; get sx from B
        add e                     ; gets sy from E
        ld  e,a
        ld  d,0                   ; DE = sx*4 + sy
        add hl,de
        ld  a,(hl)
        ld  b,a                   ; save mask into B
    __endasm;

    // addr = vti_start + gx + vti_row[yy];
    __asm
        ld   h, _vti_row / 256
        ld   l, _vti_row % 256      ; HL = vti_row

        ld   a, (_st_vti_plot_yy)
        rla
        ld   e,a
        ld   d,0                    ; DE = yy*2

        add  hl,de                  ; HL = ((unsigned char *)vti_row)[yy*2];

        ld   a,(hl)
        ld   e,a
        inc  hl
        ld   a,(hl)
        ld   d,a                    ; DE = *HL = ((unsigned char *)vti_row)[yy*2];

        ld   a,c                    ; gets gx from C
        ld   l,a
        ld   h,0                    ; HL = gx

        add  hl,de                  ; HL = gx + ((unsigned char *)vti_row)[yy*2];

        ld   a,l
        ld   (_st_vti_plot_addr),a
        ld   a,h
        ld   (_st_vti_plot_addr+1),a ; addr = HL

        ld   h, _vti_start / 256
        ld   l, _vti_start % 256    ; HL = vti_start

        ld   a, (hl)
        ld   e, a
        inc  hl
        ld   a,(hl)
        ld   d, a                   ; DE = vti_start

        ld   a,(_st_vti_plot_addr)
        ld   l,a
        ld   a,(_st_vti_plot_addr+1)
        ld   h,a

        add  hl,de

        ;HL holds addr so dont save in memory
        ;ld   a,l
        ;ld   (_st_vti_plot_addr),a
        ;ld   a,h
        ;ld   (_st_vti_plot_addr+1),a ; addr = HL

    __endasm;

    // value = *addr;
    // if (value & 0x80) value = 0x3f;
    //      if(vti_mode == VTI_MODE_RESET) { *addr = value |  mask; return; }
    // else if(vti_mode == VTI_MODE_SET)   { *addr = value & ~mask; return; }
    // else                                { *addr = value ^  mask; return; }
    __asm
        ; HL already holds addr
        ;ld  a,(_st_vti_plot_addr)
        ;ld  l,a
        ;ld  a,(_st_vti_plot_addr+1)
        ;ld  h,a                       ; HL = addr

        ld  a, (hl)                    ;
        ld  e, a                       ; save value in E

        and $80
        jp  z, goodchar                ; if a and $80 == 0 then goto goodchar
        ld  e, $3f                     ; e = $3f    (value=0x3f)
    goodchar:
        ld  a, (_vti_mode)
        cp  0
        jp  nz, case1
        ld  a,b                        ; B holds mask
        or  e
        ld  (hl),a
        ret
    case1:
        cp  1
        jp  nz, case2
        ld  a,b                        ; B holds mask
        cpl
        and e
        ld  (hl),a
        ret
    case2:
        ld  a,b                        ; B holds mask
        xor e
        ld  (hl),a
        ret
    __endasm
}

unsigned char vti_read_pixel(unsigned int x, unsigned int y) {
    static unsigned int gx;
    static unsigned char sx, sy, mask, value;
    if (x > 127 || y > 47) return 0;
    gx = x / 2;
    sx = x % 2;
    sy = vti_mod[y];
    mask = sx == 0 ? vti_pow0[sy] : vti_pow1[sy];
    value = *(vti_start + gx + vti_row[y]);
    if (value & 0x80) value = 0x3f;
    return ~value & mask ? 1 : 0;
}

void vti_box(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
    static unsigned int i;
    for (i=x0; i<=x1; ++i) {
        vti_plot(i, y0);
        vti_plot(i, y1);
    }
    for (i=y0; i<=y1; ++i) {
        vti_plot(x0, i);
        vti_plot(x1, i);
    }
}

void vti_boxfill(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
    static unsigned int i,j;
    for (i=x0; i<=x1; ++i)
        for (j=y0; j<=y1; ++j)
            vti_plot(i, j);
}

// http://members.chello.at/~easyfilter/bresenham.html
void vti_line(unsigned int _x0, unsigned int _y0, unsigned int _x1, unsigned int _y1) {

    static unsigned int x0,y0,x1,y1;
    static int dx,sx,dy,sy,err,e2;
    static unsigned char ix,iy;
    x0 = _x0;
    y0 = _y0;
    x1 = _x1;
    y1 = _y1;

    dx =  vti_abs(x1-x0);
    dy = -vti_abs(y1-y0);
    err = dx+dy; /* error value e_xy */

    ix = x0<x1;
    iy = y0<y1;

    for(;;){  /* loop */
        vti_plot(x0,y0);
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 >= dy) { err += dy; if(ix) ++x0; else --x0; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; if(iy) ++y0; else --y0; } /* e_xy+e_y < 0 */
    }
}

// http://members.chello.at/~easyfilter/bresenham.html
void vti_ellipse_rect(unsigned int _x0, unsigned int _y0, unsigned int _x1, unsigned int _y1)
{
    static unsigned int x0,y0,x1,y1;
    x0 = _x0;
    y0 = _y0;
    x1 = _x1;
    y1 = _y1;

    long a = vti_abs(x1-x0), b = vti_abs(y1-y0);
    long b1 = b&1; /* values of diameter */
    long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
    long err = dx+dy+b1*a*a, e2; /* error of 1.step */

    if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
    if (y0 > y1) y0 = y1; /* .. exchange them */
    y0 += (b+1)/2; y1 = y0-b1;   /* starting pixel */
    a *= 8*a; b1 = 8*b*b;

    do {
        vti_plot(x1, y0); /*   I. Quadrant */
        vti_plot(x0, y0); /*  II. Quadrant */
        vti_plot(x0, y1); /* III. Quadrant */
        vti_plot(x1, y1); /*  IV. Quadrant */
        e2 = 2*err;
        if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */
        if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; } /* x step */
    } while (x0 <= x1);
   
    while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
        vti_plot(x0-1, y0); /* -> finish tip of ellipse */
        vti_plot(x1+1, y0++);
        vti_plot(x0-1, y1);
        vti_plot(x1+1, y1--);
    }
}

unsigned char vti_read_char(unsigned int x, unsigned int y) {
    return *(vti_start + x + (VTI_WIDTH*y));
}

void vti_scroll_up(unsigned int n) {
    memcpy(vti_start, vti_start + (VTI_WIDTH * n), VTI_PAGESIZE - (VTI_WIDTH * n));
    memset(vti_start + VTI_PAGESIZE - (VTI_WIDTH * n), VTI_BLANK_CHAR, VTI_WIDTH*n);
}

void vti_scroll_down(unsigned int n) {
    memmove(vti_start + (VTI_WIDTH * n), vti_start, VTI_PAGESIZE - (VTI_WIDTH * n));
    memset(vti_start, VTI_BLANK_CHAR, VTI_WIDTH * n);
}

void vti_put_shape(unsigned int x, unsigned int y, char *shape, unsigned int w, unsigned int h) {
    static unsigned int xx, yy, ex, ey;
    static char ch;
    static unsigned char old_mode;
    old_mode = vti_getmode();
    ex = x+w;
    ey = y+h;
    for (yy = y; yy < ey; ++yy) {
        for (xx = x; xx < ex; ++xx) {
            ch = *shape++;
            vti_setmode(ch == '*');
            vti_plot(xx, yy);
        }
    }
    vti_setmode(old_mode);
}
