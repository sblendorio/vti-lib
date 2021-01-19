#include <stdio.h>
#include <stdlib.h>
#include "vti.h"
char get_key(void);

void bbb(int x, int y, char **shape, int w, int h) ;
void aaa(unsigned char *msg) {
	while (*msg) printf("(%d) ", *msg++);
}


void main(int argc, char *argv[]) {
    int i,j,k,m;
    char ch, *p, *q;

    if (argc >= 2) {
        q = argv[1];
        printf("--***---\n");
        long val = strtol(q, &p, 16);
        printf("Arg=%d\n", val);
        exit(1);
    }

    vti_set_start(0xe800);
    printf("Starting program...\n");
    vti_clear_screen();

    for (j=0; j<25; ++j) {
        vti_rawchar_at(j, 2, 128+j);
        vti_rawchar_at(j, 3, 153+j);
        vti_rawchar_at(j, 4, 178+j);
    }

    aaa("\177\200");
printf("\n\n");
    char *shape[] = {
        "..**.",
        ".***.",
        "..**.",
        "..**.",
        ".****",
        "....."
    };

    bbb(0,0,shape,5, 7);
    printf("...end\n");
}

char get_key(void) {
    char ch;
    while (!(ch=getk()));
    return ch;
}



void bbb(int x, int y, char **shape, int w, int h) {
    unsigned int ih, iw;
    char ch;
    for (ih = 0; ih < h; ++ih) {
        for (iw = 0; iw < w; ++iw) {
            ch = *(*(shape+ih)+iw);
            if (ch == '*' || ch =='.') printf("%c",ch);
        }
        printf("\n");
    }
}