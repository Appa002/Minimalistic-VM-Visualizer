#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <ncurses/curses.h>
#include <locale.h>

#include "header/instructions.h"
#include "header/widgets.h"
#include "header/helper.h"
#include "header/program.h"
#include "header/io.h"

static void finish(int sig) {
    endwin();
    exit(sig);
}

void init_ncurses() {
    setlocale(LC_ALL, "");
    setlocale(LC_CTYPE, "C-UTF-8");

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
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_CYAN, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);

        init_pair(8, COLOR_BLACK, COLOR_GREEN);
    }
}

void usage() {
    printf("Usage: vmv [FILE]\n");
    exit(-1);
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        usage();

    signal(SIGINT, finish);

    program_t *program = generate_program(argv[1]);

    init_ncurses();

    write_keymap_line();
    write_program(program, 0);

    move(0, 0);
    int x_pos = 0;
    int y_pos = 0;
    bool exit = false;
    int64_t first_displayed_line = 0;
    int32_t column = 0;

    while (!exit) {
        getyx(stdscr, y_pos, x_pos);
        if(first_displayed_line + y_pos < program->line_amount)
            mark_line_part(program, (uint32_t)first_displayed_line, (uint32_t)first_displayed_line + y_pos, &column);

        int c = get_key_press();
        switch (c){
            case(KEY_UP):
                scroll_up(y_pos, x_pos, &first_displayed_line, program);
                break;
            case (KEY_DOWN):
                scroll_down(y_pos, x_pos, &first_displayed_line, program);
                break;
            case (KEY_RIGHT):
                column++;
                break;
            case (KEY_LEFT):
                column--;
                break;
            case('q'):
                exit = true;
                break;
            default:
                break;
        }

        refresh();
    }
    delete_program(program);
    finish(0);
}
