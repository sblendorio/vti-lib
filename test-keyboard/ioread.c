#include <stdio.h>
#include <stdlib.h>

int port;
int dummy = 0;

void set_port(unsigned int port) __z88dk_fastcall {
#asm
    ld a,l
    ld (xportsmc+1),a
#endasm
}

unsigned char read_port(void) __z88dk_fastcall {
#asm
xportsmc:
    in a,(0xf8)
    ld l,a
    ld h,0
#endasm
}

void main(int argc, char *argv[]) {
    unsigned int base_address;

    // use a numeric argument to set the video memory start address
    if (argc >= 2) {
        port = (unsigned int) atoi(argv[1]);
    }
    else {
        printf("Usage: ioread port\r\n");
        return;
    }

    printf("Reading port %d ...", port);
    set_port(port);

    for(unsigned int t=0;t<20000;t++) {
        unsigned char n = read_port();
        dummy = n;
    }

    printf("done!\r\n");
}
