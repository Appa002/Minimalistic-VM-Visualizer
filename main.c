#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <ncurses/curses.h>
#include <locale.h>

#include "header/instructions.h"
#include "header/widgets.h"
#include "header/helper.h"
#include "header/program.h"

static void finish(int sig){
    endwin();
    exit(sig);
}

void init_ncurses(){
    setlocale(LC_ALL, "");
    setlocale(LC_CTYPE,"C-UTF-8");

    initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    nonl();         /* tell curses not to do NL->CR/NL on output */
    cbreak();       /* take input chars one at a time, no wait for \n */
    echo();         /* echo input - in color */

    if (has_colors()) {
        start_color();

        /*
         * Simple color assignment, often all we need.  Color pair 0 cannot
         * be redefined.  This example uses the same value for the color
         * pair as for the foreground color, though of course that is not
         * necessary:
         */
        init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_CYAN,    COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);

        init_pair(8, COLOR_BLACK, COLOR_GREEN);
    }
}

void usage(){
    printf("Usage: vmv [FILE]\n");
    exit(-1);
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        usage();
    }

    signal(SIGINT, finish);
    program_t *program = generate_program(argv[1]);
    init_ncurses();
    write_keymap_line();
    write_program(program);

    move(0, 0);
    int x_pos;
    int y_pos;
    bool exit = false;

    while (!exit) {
        getyx(stdscr, y_pos, x_pos);
        chtype old_c = inch();
        int c = getch();

        move(y_pos, x_pos);
        addch(old_c);
        move(y_pos, x_pos);
        switch (c){
            case ('q'):
                exit = true;
                break;
            case (KEY_UP):
                move(y_pos - 1, x_pos);
                break;
            case (KEY_DOWN):
                move(y_pos + 1, x_pos);
                break;
            default:
                break;
        }
        refresh();
    }
    delete_program(program);
    finish(0);
}
