/*
 * User interface for game  (C) 2021 Francesco Sblendorio
 * AI logic for Tic-Tac-Toe (C) 2014 Brian W. Howell
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "vti.h"

char intro_screen(void);
char get_key(void);
void init_board();
char get_digit(void);
char moveToX(char move);
char moveToY(char move);
void draw_piece(char, char, char);
void draw_board(char);
void print_turn(char);
char is_valid_move(char, char);
char put_piece(char, char, char);
char game_winner(void);
char row_wins(void);
char col_wins(void);
char diag_wins(void);
void gridCopy(char[][3] destination, char[][3] source);
void show_win(char);
void get_computer_move(void);
void computer_move(void);
char play_win(void);
char block_win(void);
char play_third_in_sequence_of_two(char);
void make_move_in_square(char, char);
char prevent_fork_scenarios(void);
void play_empty_side(void);
char play_center(void);
char play_opposite_corner(void);
char play_empty_corner(void);
void invert_board(void);
void pause(long);
void slow_print(char, char, char *);
void zero(void);
void show_strategies(void);
void init_say();
void say(char *);
void draw_usa_ussr(void);
void beep(void);

char *stty = 0x0003;

unsigned int seed;

char players_number;
char current_player;
char t;
char i,j;

char board[3][3];
char old_board[3][3];
char move, movex, movey;
char total_moves;

char winpath[6];
char next_move[2];

char player_to_move;
char matches;
long interval;
char num_of_matches;
char final_border;
char speech;

char screen_buffer[1024];

#define INTERVAL_INIT    500L
#define INTERVAL_DELTA   100L
#define INTERVAL_MESSAGE 800L
#define INTERVAL_CHARS   180L
#define INTERVAL_LINE    3000L

#define FINAL_BORDER 0
#define SPEECH 1
#define NUM_OF_MATCHES 10

char *strategies[] = {
    "USSR FIRST STRIKE",
    "U.S. FIRST STRIKE",
    "NATO / WARSAW PACT",
    "FAR EAST STRATEGY",
    "US USSR ESCALATION",
    "MIDDLE EAST WAR",
    "INDIA PAKISTAN WAR",
    "MEDITERRANEAN WAR",
    "HONGKONG VARIANT",
    "SEATO DECAPITATING",
    "CUBAN PROVOCATION",
    "INADVERTANT LAUNCH",
    "[ILLEG.] BLACKOUT",
    "ATLANTIC HEAVY",
    "CUBAN PARAMILITARY",
    "NICARAGUAN PREEMPTIVE",
    "PACIFIC TERRITORIAL",
    "BURMESE THEATERWIDE",
    "TURKISH DECOY",
    "NATO LIGHT",
    "ANGENTINA ESCALATION",
    "ICELAND MAXIMUM",
    "ARABIAN THEATERWIDE",
    "U.S. SUBVERSION",
    "AUSTRALIAN MANEUVER",
    "ARABIAN DIVERSION",
    "NORWAY LIMITED",
    "SUDAN SURPRISE",
    "NATO TERRITORIAL",
    "ZAIRE ALLIANCE",
    "ICELAND INCIDENT",
    "ENGLISH ESCALATION",
    "ZAIRE SUDDEN",
    "EGYPT PARAMILITARY",
    "MIDDLE EAST HEAVY",
    "MEXICAN TAKEOVER",
    "CHAD ALERT",
    "SAUDI MANEUVER",
    "AFRICAN TERRITORIAL",
    "ETHIOPIAN ESCALATION",
    "CANADIAN [ILLEG.]",
    "TURKISH HEAVY",
    "NATO INCURSION",
    "U.S. DEFENSE",
    "CAMBODIAN HEAVY",
    "PACT MEDIUM",
    "ARCTIC MINIMAL",
    "MEXICAN DOMESTIC",
    "TAIWAN THEATERWIDE",
    "PACIFIC MANEUVER",
    "PORTUGAL REVOLUTION",
    "ALBANIAN DECOY",
    "PALISTINIAN LOCAL",
    "MOROCCAN MINIMAL",
    "HUNGARIAN DIVERSION",
    "CZECH OPTION",
    "FRENCH ALLIANCE",
    "ARABIAN CLANDESTINE",
    "GABON REBELLION",
    "NORTHERN MAXIMUM",
    "AUSTRIAN SUPRISE",
    "NORTHERN MAXIMUM",
    "IRISH PARAMILITARY",
    "SEATO TAKEOVER",
    "HAWAIIAN ESCALATION",
    "IRANIAN MANEUVER",
    "NATO CONTAINMENT",
    "SWISS INCIDENT",
    "CUBAN MINIMAL",
    "CHAD ALERT",
    "ICELAND ESCALATION",
    "VIETNAMESE RETALIATION",
    "SYRIAN PROVOCATION",
    "LIBYAN LOCAL",
    "GABON TAKEOVER",
    "ROMANIAN WAR",
    "MIDDLE EAST OFFENSIVE",
    "DENMARK MASSIVE",
    "CHILE CONFRONTATION",
    "S.AFRICAN SUBVERSION",
    "USSR ALERT",
    "NICARAGUAN THRUST",
    "GREENLAND DOMESTIC",
    "ICELAND HEAVY",
    "KENYA OPTION",
    "PACIFIC DEFENSE",
    "UGANDA MAXIMUM",
    "THAI SUBVERSION",
    "ROMANIAN STRIKE",
    "PAKISTAN SOVEREIGNTY",
    "AFGHAN MISDIRECTION",
    "THAI VARIATION",
    "NORTHERN TERRITORIAL",
    "POLISH PARAMILITARY",
    "S.AFRICAN OFFENSIVE",
    "PANAMA MISDIRECTION",
    "SCANDINAVIAN DOMESTIC",
    "JORDAN PREEMPTIVE",
    "ENGLISH THRUST",
    "BURMESE MANEUVER",
    "SPAIN COUNTER",
    "ARABIAN OFFENSIVE",
    "CHAD INTERDICTION",
    "TAIWAN MISDIRECTION",
    "BANGLADESH THEATERWIDE",
    "ETHIOPIAN LOCAL",
    "ITALIAN TAKEOVER",
    "VIETNAMESE INCIDENT",
    "ENGLISH PREEMPTIVE",
    "DENMARK ALTERNATE",
    "THAI CONFRONTATION",
    "TAIWAN SURPRISE",
    "BRAZILIAN STRIKE",
    "VENEZUELA SUDDEN",
    "MAYLASIAN ALERT",
    "ISREAL DISCRETIONARY",
    "LIBYAN ACTION",
    "PALISTINIAN TACTICAL",
    "NATO ALTERNATE",
    "CYPRESS MANEUVER",
    "EGYPT MISDIRECTION",
    "BANGLADESH THRUST",
    "KENYA DEFENSE",
    "BANGLADESH CONTAINMENT",
    "VIETNAMESE STRIKE",
    "ALBANIAN CONTAINMENT",
    "GABON SURPRISE",
    "IRAQ SOVEREIGNTY",
    "VIETNAMESE SUDDEN",
    "LEBANON INTERDICTION",
    "TAIWAN DOMESTIC",
    "ALGERIAN SOVEREIGNTY",
    "ARABIAN STRIKE",
    "ATLANTIC SUDDEN",
    "MONGOLIAN THRUST",
    "POLISH DECOY",
    "ALASKAN DISCRETIONARY",
    "CANADIAN THRUST",
    "ARABIAN LIGHT",
    "S.AFRICAN DOMESTIC",
    "TUNISIAN INCIDENT",
    "MAYLASIAN MANEUVER",
    "JAMAICA DECOY",
    "MAYLASIAN MINIMAL",
    "RUSSIAN SOVEREIGNTY",
    "CHAD OPTION",
    "BANGLADESH WAR",
    "BURMESE CONTAINMENT",
    "ASIAN THEATERWIDE",
    "BULGARIAN CLANDESTINE",
    "GREENLAND INCURSION",
    "EGYPT SURGICAL",
    "CZECH HEAVY",
    "TAIWAN CONFRONTATION",
    "GREENLAND MAXIMUM",
    "UGANDA OFFENSIVE",
    "CASPIAN DEFENSE",
    NULL
};

void main(int argc, char *argv[]) {
    char ch;
    char *s;
    unsigned int base_address;
    char outcome;

    seed = 0;

    // use a numeric argument to set the video memory start address
    if (argc >= 2) {
        base_address = (unsigned int) atoi(argv[1]);
        if (base_address == 0) base_address = 0xe800;
        vti_set_start(base_address);
    }

    num_of_matches = NUM_OF_MATCHES;
    if (argc >= 3) {
        num_of_matches = (char) atoi(argv[2]);
    }

    final_border = FINAL_BORDER;
    if (argc >= 4) {
        final_border = (char) atoi(argv[3]);
    }

    speech = SPEECH;
    if (argc >= 5) {
        speech = (char) atoi(argv[4]);
    }

    printf("\032");
    for(;;) {
        players_number = intro_screen();
        interval = INTERVAL_INIT;

        if (players_number == 0) {
            zero();
        }

        for (matches=0; (players_number == 0 & matches < num_of_matches) || (players_number != 0 && matches < 1); ++matches) {
            vti_clear_screen();
            draw_board(players_number);

            init_board();
            total_moves = 0;
            t = 0;
            do {
                outcome = -1;
                current_player = t+1;

                //if (players_number > 0)
                print_turn(current_player);

                if (players_number <= 1 && current_player == 2) {
                    if (players_number == 0) pause(interval);
                    invert_board();
                    player_to_move = current_player == 1 ? 2 : 1;
                    get_computer_move();
                    movex = next_move[0];
                    movey = next_move[1];
                    invert_board();
                } else if (players_number == 0 && current_player == 1 && total_moves == 0) {
                    pause(interval);
                    movex = abs(rand()) % 3;
                    movey = abs(rand()) % 3;
                } else if (players_number == 0 && current_player == 1 && total_moves != 0) {
                    pause(interval);
                    player_to_move = current_player;
                    get_computer_move();
                    movex = next_move[0];
                    movey = next_move[1];
                } else if (players_number == 2 || (players_number == 1 && current_player == 1)) {
                    move = get_digit();
                    if (move == 0) break;
                    movex = moveToX(move);
                    movey = moveToY(move);
                    if (!is_valid_move(movex, movey)) continue;
                }

                draw_piece(current_player, movex, movey);
                if (put_piece(current_player, movex, movey))
                    ++total_moves;

                t = 1-t;
                outcome = game_winner();
            } while (outcome == -1);
            if (outcome == -1) continue;

            vti_print_at(51, 7, "             ");
            s = outcome == 0
                ? " STALEMATE." : outcome == 1
                ? "Winner is X"
                : "Winner is O";
            vti_print_at(51, 7, s);

            if (outcome > 0) show_win(outcome);

            if (players_number != 0) {
                vti_setmode(VTI_MODE_INVERT);
                for (t=0; t<20; ++t)
                    vti_box(99, 19, 127, 25);
                vti_setmode(VTI_MODE_SET);
                vti_box(99, 19, 127, 25);
                get_key();
            } else {
                interval -= INTERVAL_DELTA;
                if (interval < 0) interval = 0;
                // vti_setmode(VTI_MODE_SET);
                // vti_box(99, 19, 127, 25);
                pause(INTERVAL_MESSAGE);
            }
        }
        if (players_number == 0) {
            vti_save_screen(screen_buffer);
            for (t=0; t<20; ++t) {
                vti_clear_screen();
                pause(100);
                vti_restore_screen(screen_buffer);
                pause(100);
            }
            vti_fill_screen(0);
            pause(800);
            vti_clear_screen();
            beep();
            pause(INTERVAL_LINE);
            pause(INTERVAL_LINE);
            draw_usa_ussr();
            show_strategies();
            if (final_border) {
                vti_box(0, 0, 127, 47);
                vti_box(1, 0, 126, 47);
            }
            pause(INTERVAL_LINE);
            pause(INTERVAL_LINE);
            printf("\032");
            init_say();
            say("Greetings professor Falken");
            slow_print(3, 2, "GREETINGS PROFESSOR FALKEN");
            pause(INTERVAL_LINE);
            slow_print(3, 4, "HELLO");
            pause(INTERVAL_LINE);
            say("A strange game.");
            slow_print(3, 6, "A STRANGE GAME.");
            say("The only winning move is");
            slow_print(3, 7, "THE ONLY WINNING MOVE IS");
            say("Not to play.");
            slow_print(3, 8, "NOT TO PLAY.");
            pause(INTERVAL_LINE);
            say("How about a nice game of chess?");
            slow_print(3, 10, "HOW ABOUT A NICE GAME OF CHESS?");
            get_key();
        }
    }
}

char intro_screen(void) {
    static char ch;
    static char buf[20];
    ch = ' ';
    vti_clear_screen();
    vti_setmode(VTI_MODE_SET);
    vti_box(0, 0, 127, 47);
    vti_box(1, 0, 1,   47);
    vti_box(126,0, 126,47);
    vti_center_at(1, "Professor Falken's TIC-TAC-TOE for IMSAI 8080");
    vti_center_at(2, "---------------------------------------------");
    vti_center_at(3, "How many players?");
    vti_center_at(12, "PRESS \".\" TO EXIT");
    vti_center_at(14, "(C) Francesco Sblendorio 2021");
    vti_line(56, 14, 56, 33);
    vti_line(57, 14, 57, 33);
    
    vti_line(68, 14, 68, 33);
    vti_line(69, 14, 69, 33);

    vti_line(46, 20, 79, 20);
    vti_line(46, 27, 79, 27);

    vti_put_shape(50, 14,
        ".*."
        "**."
        ".*."
        ".*."
        "***",
        3, 5
    );

    vti_put_shape(60, 22,
        ".****."
        "*....*"
        "*....*"
        ".****.",
        6, 4);

    vti_put_shape(72, 29,
        ".***."
        "*...*"
        "..**."
        ".*..."
        "*****",
        5, 5);
    
    do {
        ch = get_key();
    } while (ch != '0' && ch != '1' && ch != '2' && ch != '.');
    if (ch == '.') {
        vti_clear_screen();
        exit(0);
    }

    srand(seed);
    return ch - '0';
}

char get_key(void) {
    char ch;
    while (!(ch=getk())) ++seed;
    return ch & 0x7f;
}

char get_digit(void) {
    char ch;
    do {
        ch = get_key();
        if (ch == '.') ch = '0';
    } while (ch < '0' || ch > '9');

    return ch - '0';
}

void init_board() {
    char i,j;
    for (i=0; i<3; ++i) for (j=0; j<3; ++j) board[i][j] = 0;
}

char moveToX(char move) {
    static char posx[] = {0, 0, 1, 2, 0, 1, 2, 0, 1, 2};
    return posx[move];
}

char moveToY(char move) {
    static char posy[] = {0, 2, 2, 2, 1, 1, 1, 0, 0, 0};
    return posy[move];
}

void draw_piece(char player, char posx, char posy) {
    char xx, yy;
    xx = (posx*32) + 0;
    yy = (posy*17) + 0;

    if (player == 1) {
        vti_line(xx+0, yy, xx+24+0, yy+13);
        vti_line(xx+1, yy, xx+24+1, yy+13);

        vti_line(xx+0, yy+13, xx+24+0, yy);
        vti_line(xx+1, yy+13, xx+24+1, yy);
    } else if (player == 2) {
        vti_ellipse_rect(xx+0, yy, xx+24+0, yy+13);
        vti_ellipse_rect(xx+1, yy, xx+24+1, yy+13);
    }
}

char put_piece(char player, char x, char y) {
    char j = board[x][y];
    if (!j) board[x][y] = player;
    return !j;
}

void draw_board(char number_of_players) {
    vti_line(0, 15, 89, 15);
    vti_line(0, 32, 89, 32);

    vti_line(28, 0, 28, 47);
    vti_line(29, 0, 29, 47);

    vti_line(60, 0, 60, 47);
    vti_line(61, 0, 61, 47);

    if (number_of_players != 0) {
        vti_print_at(55, 0, "Move with");
        vti_print_at(55, 2, "  7 8 9");
        vti_print_at(55, 3, "  4 5 6");
        vti_print_at(55, 4, "  1 2 3");
    }

}

void print_turn(char player) {
    char *s;
    s = (player == 1) ? "Player X turn" : "Player O turn";
    vti_print_at(51, 7, s);
}

char is_valid_move(char x, char y) {
    return !board[x][y];
}

char game_winner(void) {
    char row_win, col_win, diag_win;
    
    row_win = row_wins();
    if (row_win) return row_win;

    col_win = col_wins();
    if (col_win) return col_win;

    diag_win = diag_wins();
    if (diag_win) return diag_win;

    if (total_moves == 9) return 0;
    return -1;
}

char row_wins(void) {
    for ( int i = 0; i < 3; i++ ) {
        int xCount = 0;
        int oCount = 0;
        for ( int j = 0; j < 3; j++ ) {
            if ( board[i][j] == 1 ) ++xCount;
            if ( board[i][j] == 2 ) ++oCount;
        }
        if ( xCount == 3 || oCount == 3 ) {
            winpath[0] = i;
            winpath[1] = 0;
            winpath[2] = i;
            winpath[3] = 1;
            winpath[4] = i;
            winpath[5] = 2;
            if ( xCount == 3 ) return 1;
            if ( oCount == 3 ) return 2;
        }
    }
    return 0;
}

char col_wins() {
    for ( int i = 0; i < 3; i++ ) {
        int xCount = 0;
        int oCount = 0;
        for ( int j = 0; j < 3; j++ ) {
            if ( board[j][i] == 1 ) ++xCount;
            if ( board[j][i] == 2 ) ++oCount;
        }
        if ( xCount == 3 || oCount == 3 ) {
            // winPath.setPath( 0, i, 2, i);
            winpath[0] = 0;
            winpath[1] = i;
            winpath[2] = 1;
            winpath[3] = i;
            winpath[4] = 2;
            winpath[5] = i;
            if ( xCount == 3 ) return 1;
            if ( oCount == 3 ) return 2;
        }
    }
    return 0;        }

}

char diag_wins() {
    if ( board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1 ) {
        winpath[0] = 0;
        winpath[1] = 0;
        winpath[2] = 1;
        winpath[3] = 1;
        winpath[4] = 2;
        winpath[5] = 2;
        return 1;
    } else if  ( board[2][0] == 1 && board[1][1] == 1 && board[0][2] == 1 ) {
        winpath[0] = 0;
        winpath[1] = 2;
        winpath[2] = 1;
        winpath[3] = 1;
        winpath[4] = 2;
        winpath[5] = 0;
        return 1;
    } else if  ( board[0][0] == 2 && board[1][1] == 2 && board[2][2] == 2 ) {
        winpath[0] = 0;
        winpath[1] = 0;
        winpath[2] = 1;
        winpath[3] = 1;
        winpath[4] = 2;
        winpath[5] = 2;
        return 2;
    } else if  ( board[2][0] == 2 && board[1][1] == 2 && board[0][2] == 2 ) {
        winpath[0] = 0;
        winpath[1] = 2;
        winpath[2] = 1;
        winpath[3] = 1;
        winpath[4] = 2;
        winpath[5] = 0;
        return 2;
    } else {
        return 0;
    }
} 


void gridCopy(char[][3] destination, char[][3] source) {
    for (int i=0; i<3; ++i) for (int j=0; j<3; ++j) destination[i][j] = source[i][j];
}

void show_win(char player) {
    for (j=0; j<6; ++j) {
        vti_setmode(j % 2 == 0 ? VTI_MODE_RESET : VTI_MODE_SET);
        for (i=0; i<6; i+=2) {
            draw_piece(player, winpath[i], winpath[i+1]);
        }
    }
    vti_setmode(VTI_MODE_SET);
}

void get_computer_move() {
    gridCopy(old_board, board);
    computer_move();
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3 ; ++j) {
            if (board[i][j] != old_board[i][j]) {
                next_move[0] = i;
                next_move[1] = j;
                return;
            }
        }
    }
    next_move[0] = 0;
    next_move[1] = 0;
}

void computer_move() {
    if (play_win()) return;                            // always plays win
    if (block_win()) return;  // always blocks when hard
    if (prevent_fork_scenarios()) return; // always prevents forks when hard
    if (play_center()) return;
    if (play_opposite_corner()) return;
    if (play_empty_corner()) return;
    play_empty_side();
}

char play_win() {
    return play_third_in_sequence_of_two(2);
}

char block_win() {
    return play_third_in_sequence_of_two(1);
}

char play_third_in_sequence_of_two(char playedBy) {
    if ( total_moves < 3 ) return 0; // only check after 4 moves have been made

    // Search rows for 2 plus empty square.
    for ( int i = 0; i < 3; i++ ) {
        int count = 0;
        int emptyCount = 0;
        for ( int j = 0; j < 3; j++ ) {
            if ( board[i][j] == playedBy ) ++count;
            if ( board[i][j] == 0) ++emptyCount;
        }
        if ( count == 2 && emptyCount == 1 ) {
            for ( int j = 0; j < 3; j++ ) {
                if ( board[i][j] == 0) make_move_in_square( i, j );
            }
            return 1;
        }
    } // end row search

    // Search columns for 2 plus empty square.
    for ( int i = 0; i < 3; i++ ) {
        int count = 0;
        int emptyCount = 0;
        for ( int j = 0; j < 3; j++ ) {
            if ( board[j][i] == playedBy ) ++count;
            if ( board[j][i] == 0) ++emptyCount;
        }
        if ( count == 2 && emptyCount == 1 ) {
            for ( int j = 0; j < 3; j++ ) {
                if ( board[j][i] == 0) make_move_in_square( j, i );
            }
            return 1;
        }
    } // end column search

    // Search downward diagonal for 2 plus empty square.
    int count = 0;
    int emptyCount = 0;
    for ( int i = 0, j = 0; i < 3; ++i, ++j )
    {
        if ( board[i][j] == playedBy ) ++count;
        if ( board[i][j] == 0) ++emptyCount;
        if ( count == 2 && emptyCount == 1 ) {
            if ( board[0][0] == 0) make_move_in_square( 0, 0 );
            if ( board[1][1] == 0) make_move_in_square( 1, 1 );
            if ( board[2][2] == 0) make_move_in_square( 2, 2 );
            return 1;
        }
    } // end downward diagonal search

    // Search upward diagonal for 2 plus empty square.
    count = 0; emptyCount = 0;
    for (int i = 0, j = 2; i < 3; i++, j--)
    {
        if ( board[i][j] == playedBy ) ++count;
        if ( board[i][j] == 0) ++emptyCount;
        if ( count == 2 && emptyCount == 1 ) {
            if ( board[2][0] == 0) make_move_in_square( 2, 0 );
            if ( board[1][1] == 0) make_move_in_square( 1, 1 );
            if ( board[0][2] == 0) make_move_in_square( 0, 2 );
            return 1;
        }
    } // end upward diagonal search

    return 0;
} // end play_third_in_sequence_of_two( char playedBy )

void make_move_in_square( char row, char col ) {
    board[row][col] = player_to_move;           // make the move in the game board model

    if ( player_to_move == 1 )                    // update the player to move
        player_to_move = 2;
    else if ( player_to_move == 2 )
        player_to_move = 1;

    ++total_moves;                                  // count the move
}

char prevent_fork_scenarios() {
    if ( total_moves == 3 ) {
        if ( board[0][0] == 1 && board[1][1] == 2 && board[2][2] == 1 ) {
            play_empty_side();
            return 1;
        }
        if ( board[2][0] == 1 && board[1][1] == 2 && board[0][2] == 1 ) {
            play_empty_side();
            return 1;
        }
        if ( board[2][1] == 1 && board[1][2] == 1 ) {
            make_move_in_square( 2, 2 );
            return 1;
        }
    }
    return 0;
}

void play_empty_side()
{
    if ( board[0][1] == 0 ) {
        make_move_in_square( 0, 1 );
    } else if ( board[1][0] == 0 ) {
        make_move_in_square( 1, 0 );
    } else if ( board[1][2] == 0 ) {
        make_move_in_square( 1, 2 );
    } else if ( board[2][1] == 0 ) {
        make_move_in_square( 2, 1 );
    }
}

char play_center() {
    if ( board[1][1] == 0) {
        make_move_in_square( 1, 1 );
        return 1;
    }
    return 0;
}

char play_opposite_corner()
{
    if ( board[0][0] == 1 && board[2][2] == 0) {
        make_move_in_square( 2, 2 );
        return 1;
    } else if ( board[2][2] == 1 && board[0][0] == 0) {
        make_move_in_square( 0, 0 );
        return 1;
    } else if ( board[0][2] == 1 && board[2][0] == 0) {
        make_move_in_square( 2, 0 );
        return 1;
    } else if ( board[2][0] == 1 && board[0][2] == 0) {
        make_move_in_square( 0, 2 );
        return 1;
    }
    return 0;
}

char play_empty_corner()
{
    if ( board[0][0] == 0) {
        make_move_in_square( 0, 0 );
        return 1;
    } else if ( board[0][2] == 0) {
        make_move_in_square( 0, 2 );
        return 1;
    } else if ( board[2][0] == 0) {
        make_move_in_square( 2, 0 );
        return 1;
    } else if ( board[2][2] == 0) {
        make_move_in_square( 2, 2 );
        return 1;
    }
    return 0;
}

void invert_board(void) {
    int i,j;
    for (i=0; i<3; ++i)
        for (j=0; j<3; ++j)
            if (board[i][j] == 1)
                board[i][j] = 2;
            else if (board[i][j] == 2)
                board[i][j] = 1;
}

void pause(long n) {
    for (long i=0; i <= n; ++i) {
        // do nothing
    }
}

void slow_print(char x, char y, char *s) {
    static char *msg = "A";
    static char i;
    for (i=0; i<strlen(s); ++i) {
        msg[0] = s[i];
        vti_print_at(x+i, y, msg);
        pause(INTERVAL_CHARS);
    }
}

void zero(void) {
    vti_clear_screen();

    vti_rawchar_at(0, 0, 0);
    pause(1 * INTERVAL_LINE);

    vti_print_at(0, 0, "Z");
    vti_rawchar_at(1, 0, 0);
    pause(3 * INTERVAL_CHARS);

    vti_print_at(0, 0, "ZE");
    vti_rawchar_at(2, 0, 0);
    pause(3 * INTERVAL_CHARS);

    vti_print_at(0, 0, "ZER");
    vti_rawchar_at(3, 0, 0);
    pause(3 * INTERVAL_CHARS);

    vti_print_at(0, 0, "ZERO");
    vti_rawchar_at(4, 0, 0);
    pause(3 * INTERVAL_CHARS);

    pause(1 * INTERVAL_LINE);
}

void show_strategies(void) {
    int i = 0;
    int pause = 100;
    int delta = 1;
    int pagesize = 10;
    int row = 0;
    while (strategies[i] != NULL) {
        if (i % pagesize == 0) {
            vti_clear_screen();
            vti_print_at( 3, 2, "STRATEGY:");
            vti_print_at(40, 2, "WINNER:");
            row = 4;
            if (i == 0) msleep(1000);
        }
        vti_print_at( 3, row, strategies[i]);
        if (i <= 1) msleep(600); else
            if (pause > 0) { msleep(pause); if (pause >= delta) pause -= delta; } else msleep(10);
        vti_print_at(43, row, "NONE");
        if (i == 0) msleep(600); else
            if (pause > 0) { msleep(pause); if (pause >= delta) pause -= delta; } else msleep(10);

        ++row;

        ++i;
    }
    msleep(1000);
    vti_clear_screen();
}

void init_say() {
    if (!speech) return;
    *stty = 3;
    printf("%s\n", "@=2 @w2 @f7 @r2");
    *stty = 1;
}

void say(char *msg) {
    if (!speech) return;
    *stty = 3;
    printf("%s\n", msg);
    *stty = 1;
}

void beep() {
    if (!speech) return;
    printf("\007");
}

void draw_usa_ussr() {
    printf("%s",
        "\032"
        "          ,------~~v,__   _____                     ____--^\\\n"
        "         /             \\,/     /           ,,   ,,/^      Z  vZv-__\n"
        "         |                    /            |'~^Z                   Z\\\n"
        "         \\                   |           _/                     _  /^\n"
        "          \\                 /           /                   ,~~^/|ZZ\n"
        "           ^Z~_            /            |          __,,  v__\\   \\/\n"
        "               '~~,  ,Z~Z\\ \\             ^~       /    ~Z  //\n"
        "                   \\/     \\/               \\~,  ,/         Z\n"
        "                                              ~~\n"                         
        "            UNITED STATES                     SOVIET UNION\n"
        "\n"
        "  TRAJECTORY HEADING TRAJECTORY HEADING TRAJECTORY HEADING TRAJECTORY HEADING\n"
        "  ------------------ ------------------ ------------------ ------------------\n"
        "  A-SS20-A 929 523   C-SS20-A 984 675   E-SS20-A 398 984   G-SS20-A 919 437\n"
        "         B 664 295          B 892 754          B 982 762          B 132 147\n"
        "         C 125 386          C 374 256          C 276 674          C 050 195\n"
        "         D 496 374          D 826 684          D 251 953          D 135 404\n"
        "         E 987 384          E 873 543          E 352 601          E 461 284\n"
        "\n"
        "  B-SS20-A 495 896   D-SS20-A 365 590   F-SS20-A 574 651   H-SS20-A 959 913\n"
        "         B 490 485          B 778 974          B 230 953          B 135 728\n"
        "         C 875 273          C 210 852          C 321 615          C 952 464\n"
        "         D 239 385          D 861 557          D 065 481          D 281 366\n"
        "         E 978 253          E 862 007          E 153 249          E 684 420"
    );
}