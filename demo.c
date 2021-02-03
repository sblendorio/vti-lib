#include <stdio.h>
#include <stdlib.h>
#include "vti.h"
char get_key(void);

void main(int argc, char *argv[]) {
    int i,j,k,m;
    char ch;
    unsigned int base_address;

    // use a numeric argument to set the video memory start address
    if (argc >= 2) {
        base_address = (unsigned int) atoi(argv[1]);
        if(base_address == 0) base_address = 0xe800;
        vti_set_start(base_address);
    }

    printf("Starting program...\n");
    
    // *************************************************************************
    vti_clear_screen();
    vti_print_at(0, 0, "VTI Retrocampus test - by Francesco Sblendorio");
    vti_print_at(0, 1, "----------------------------------------------");

    vti_print_at(0, 3, "Letters:");
    vti_print_at(0, 4, "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz");

    vti_print_at(0, 6, "Numbers:");
    vti_print_at(0, 7, "0123456789");

    vti_print_at(0, 9, "Symbols");
    vti_print_at(0,10, "!\"#$%&'()*+,-./:;<=>?@[\\]^_{|}~`");
    vti_print_at(0,11, "\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217"
                       "\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237");
    vti_print_at(0, 14, "- PRESS A KEY TO CONTINUE -");
    get_key();
    for (i=0; i<16; ++i) {
        vti_scroll_down(1);
        csleep(5);
    }

    // *************************************************************************
    vti_clear_screen();
    vti_print_at(0, 0, "Let's draw");
    m=0;
    for (k=0; k<5; ++k) {
        vti_setmode(k&1 ? VTI_MODE_RESET : VTI_MODE_SET);
        vti_boxfill(k, k+5, k+20, k+15);
    }
    vti_print_at(0, 14, "- PRESS A KEY TO CONTINUE -");
    get_key();
    for (i=0; i<16; ++i) {
        vti_scroll_up(1);
        csleep(5);
    }

    // *************************************************************************
    vti_clear_screen();
    vti_print_at(0, 0, "And now some shapes");
    vti_box(5, 5, 60, 37);
    vti_line(5, 5, 60, 37);
    vti_ellipse_rect(7, 7, 58, 35);
    vti_print_at(0, 14, "- PRESS A KEY TO CONTINUE -");
    get_key();
    for (i=0; i<16; ++i) {
        vti_scroll_down(1);
        csleep(5);
    }
    vti_clear_screen();

    // *************************************************************************
    vti_clear_screen();
    vti_print_at(0, 0, "And a diamond");
    for(i=0; i<64; i+=5) {
        vti_line(64,  0, 64+i, 24);
        vti_line(64,  0, 64-i, 24);
        vti_line(64, 47, 64+i, 24);
        vti_line(64, 47, 64-i, 24);
    }
    vti_print_at(0, 15, "- PRESS A KEY TO CONTINUE -");
    get_key();
    for (i=0; i<16; ++i) {
        vti_scroll_down(1);
        csleep(5);
    }
    vti_clear_screen();

    // *************************************************************************
    vti_print_at(0, 0, "Let's test XOR mode");
    vti_ellipse_rect(7, 7, 40, 25);
    vti_setmode(VTI_MODE_INVERT);
    for (k=0; k<2; ++k) vti_boxfill(5, 5, 42, 27);
    vti_setmode(VTI_MODE_SET);
    vti_print_at(0, 14, "- PRESS A KEY TO CONTINUE -");
    get_key();
    for (i=0; i<16; ++i) {
        vti_scroll_down(1);
        csleep(5);
    }

    // *************************************************************************
    vti_clear_screen();
    vti_print_at(20, 0, "Some animation");
    char *frame1 =
        "........"
        "..****.."
        ".******."
        ".*****.."
        ".****..."
        ".*****.."
        ".******."
        "..****.."
        "........"
    ;
    char *frame2 =
        "........"
        "..****.."
        ".******."
        ".******."
        ".******."
        ".******."
        ".******."
        "..****.."
        "........"
    ;
    char *frame3 =
        "........"
        "..****.."
        ".******."
        ".*.**.*."
        ".******."
        ".*....*."
        ".**..**."
        "..****.."
        "........"
    ;
    char *p;
    for (i=0; i<30; ++i) {
        p = i&1?frame2:frame1;
        vti_put_shape(i,5,p,8,9);
        csleep(5);
    }
    vti_put_shape(i,5,frame3,8,9);
    vti_print_at(0, 15, "- GOODBYE! -");

    printf("...end\n");
}

char get_key(void) {
    char ch;
    while (!(ch=getk()));
    return ch;
}
