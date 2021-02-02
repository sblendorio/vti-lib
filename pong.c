#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "vti.h"

#define IN_KEYBOARD 0xe8

__sfr __at (IN_KEYBOARD) PORT;

void randomize_ball(void);
char get_key(void);
void draw_field(void);
void player1_up(void);
void player1_down(void);    
void player2_up(void);
void player2_down(void);
char move_ball(void);
void update_score(void);
void intro_screen(void);

#define LEN 20

#define LOWER_X 1
#define UPPER_X 126
#define LOWER_Y 3
#define UPPER_Y 47

char pos_a, pos_b;

char ball_x, ball_y, prev_ball_x, prev_ball_y, ball_step, dx, dy;
unsigned int score_a, score_b;
unsigned int seed;
unsigned char count;
unsigned char speed = 7;

char *boom =
    "....********...."
    "..************.."
    ".**...****...**."
    ".**...****...**."
    ".**************."
    "..*****..*****.."
    "..*****..*****.."
    "...**********..."
    "...**..**..**..."
    "...**..**..**..."
;

void main(int argc, char *argv[]) {
    char ch;
    unsigned int base_address;
    char outcome;

    seed = 0;

    // use a numeric argument to set the video memory start address
    if (argc >= 2) {
        base_address = (unsigned int) atoi(argv[1]);
        if (base_address == 0) base_address = 0xe800;
        vti_set_start(base_address);
    }

    #ifndef NOBARS
    for (;;) {
    #endif

        score_a = score_b = 0;
        pos_a = pos_b = LOWER_Y;
        ball_step = 0;
        prev_ball_x = prev_ball_y = -1;
        randomize_ball();

        #ifndef NOBARS
        intro_screen();
        #endif

        vti_clear_screen();
        draw_field();

        for (;;)  {
            #ifdef VTI_KEYBOARD
            ch = vti_read_keyboard() ^ 0x80;
            #else
            ch = getk();
            #endif
            ch = tolower(ch);
            if (ch == '.') {
                break;
            }
            #ifndef NOBARS
            else if (ch == 'q') {
                player1_up();
            } else if (ch == 'a') {
                player1_down();
            } else if (ch == 'p') {
                player2_up();
            } else if (ch == 'l') {
                player2_down();
            }
            #endif

            ++count;
            count %= speed;
            if (!count) {
                outcome = move_ball();
                #ifndef NOBARS
                if (outcome) {
                    if (outcome == 1) {
                        ++score_a;
                        vti_put_shape(100, 10, boom, 16, 10);
                        csleep(10);
                        vti_setmode(VTI_MODE_RESET);
                        vti_boxfill(100, 10, 116, 20);
                    } else {
                        ++score_b;
                        vti_put_shape(11, 10, boom, 16, 10);
                        csleep(10);
                        vti_setmode(VTI_MODE_RESET);
                        vti_boxfill(11, 10, 11+16, 20);
                    }
                    update_score();
                    randomize_ball();
                }
                #endif
            }
        }

    #ifndef NOBARS
    }
    #endif
}

void randomize_ball(void) {
    ball_x = (rand() % 10) -5 + 64;
    ball_y = (rand() % 10) -5 + 24;
    dx = (rand() % 2) ? -1 : 1;
    dy = (rand() % 2) ? -1 : 1;
}

char move_ball(void) {
    prev_ball_x = ball_x;
    prev_ball_y = ball_y;
    static char next_y;

    next_y = ball_y + dy;
    if (ball_x == UPPER_X) {
        #ifndef NOBARS
        if ((next_y < pos_b && next_y >= LOWER_Y) || (next_y >= pos_b + LEN && next_y <= UPPER_Y)) {
            vti_setmode(VTI_MODE_RESET);
            vti_plot(prev_ball_x, prev_ball_y);
            return 1;
        }
        #endif
        dx = -1;
    } else if (ball_x == LOWER_X) {
        #ifndef NOBARS
        if ((next_y < pos_a && next_y >= LOWER_Y) || (next_y >= pos_a + LEN && next_y <= UPPER_Y)) {
            vti_setmode(VTI_MODE_RESET);
            vti_plot(prev_ball_x, prev_ball_y);
            return 2;
        }
        #endif
        dx = 1;
    }
    if (ball_y == UPPER_Y) {
        dy = -1;
    } else if (ball_y == LOWER_Y) {
        dy = 1;
    }
    ball_x += dx;
    ball_y += dy;

    vti_setmode(VTI_MODE_SET);   vti_plot(ball_x, ball_y);
    vti_setmode(VTI_MODE_RESET); vti_plot(prev_ball_x, prev_ball_y);

    return 0;
}

void player1_up() {
    if (pos_a == LOWER_Y) return;
    vti_setmode(VTI_MODE_RESET); vti_plot(0, pos_a + LEN);
    --pos_a;
    vti_setmode(VTI_MODE_SET); vti_plot(0, pos_a);
}

void player1_down() {
    if (pos_a == 47-LEN) return;
    vti_setmode(VTI_MODE_RESET); vti_plot(0, pos_a);
    ++pos_a;
    vti_setmode(VTI_MODE_SET); vti_plot(0, pos_a + LEN);
}

void player2_up() {
    if (pos_b == LOWER_Y) return;
    vti_setmode(VTI_MODE_RESET); vti_plot(127, pos_b + LEN);
    --pos_b;
    vti_setmode(VTI_MODE_SET); vti_plot(127, pos_b);
}

void player2_down() {
    if (pos_b == 47-LEN) return;
    vti_setmode(VTI_MODE_RESET); vti_plot(127, pos_b);
    ++pos_b;
    vti_setmode(VTI_MODE_SET); vti_plot(127, pos_b + LEN);
}

void draw_field(void) {    score_a = score_b = 0;
    pos_a = pos_b = LOWER_Y;
    ball_step = 0;
    prev_ball_x = prev_ball_y = -1;
    ball_x = 7;
    ball_y = 7;
    dx = 1;
    dy = 1;

    vti_setmode(VTI_MODE_SET);
    vti_plot(ball_x, ball_y);

    #ifndef NOBARS
    vti_line(0, pos_a, 0, pos_a+LEN);
    vti_line(127, pos_a, 127, pos_a+LEN);
    vti_center_at(0, "IMSAI Pong");
    update_score();
    #endif
}

void update_score(void) {
    static char buffer[20];
    itoa(score_a, buffer, 10);
    vti_print_at(0, 0, buffer);
    itoa(score_b, buffer, 10);
    vti_print_at(64 - strlen(buffer), 0, buffer);
}

void intro_screen(void) {
    static char ch;
    static char buf[20];
    ch = ' ';
    vti_clear_screen();
    vti_setmode(VTI_MODE_SET);
    vti_box(2, 1, 125, 46);
    vti_center_at(1, "IMSAI Pong - (C) Francesco Sblendorio 2021");
    vti_center_at(2, "------------------------------------------");
    vti_center_at(4, "Keys");
    vti_center_at(5, "Left Player              Right Player");
    vti_center_at(7, "       Up Q              P Up        ");
    vti_center_at(8, "     Down A              L Down      ");
    vti_center_at(12,"Use +/- to change speed, \".\" to EXIT");
    vti_center_at(14, "PRESS A KEY TO START");
    do {
        sprintf(buf, " Slow down: %d ", speed);
        vti_center_at(10, buf);
        ch = get_key();
        if (ch == '+' && speed < 15) ++speed;
        else if (ch == '-' && speed > 1) --speed;
    } while (ch == '+' || ch == '-');
    if (ch == '.') {
        vti_clear_screen();
        exit(0);
    }

    srand(seed);
}

char get_key(void) {
    char ch;
    #ifdef VTI_KEYBOARD
    while (vti_keypressed()) ++seed;
    while (!((ch=vti_read_keyboard()) & 0x80)) ++seed;
    #else
    while (!(ch=getk())) ++seed;
    #endif
    return ch & 0x7f;
}
