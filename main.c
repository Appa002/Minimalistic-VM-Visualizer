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

void finish(void) {
    endwin();
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
        init_pair(9, COLOR_BLACK, COLOR_WHITE);
    }
}

void usage() {
    printf("Usage: vmv [FILE]\n");
    exit(-1);
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        usage();

    atexit(finish);
    program_t *program = generate_program(argv[1]);

    init_ncurses();

    write_keymap_line();
    write_representation_line(0, program, 0);
    write_program(program, 0);

    move(0, 0);
    int x_pos = 0;
    int y_pos = 0;
    bool exit = false;
    uint32_t scrolled_to_line_index = 0;
    int32_t column = 0;
    uint32_t selected_line_index;

    while (!exit) {
        selected_line_index = scrolled_to_line_index + y_pos;

        if(selected_line_index < program->line_amount){
            mark_line_part(program, y_pos, selected_line_index, &column);
            write_representation_line(selected_line_index, program, (uint32_t)column);
        }

        int c = get_key_press();
        switch (c){
            case(KEY_UP):
                move_cursor_up(&scrolled_to_line_index, program, &y_pos, &x_pos);
                break;
            case (KEY_DOWN):
                move_cursor_down(&scrolled_to_line_index, program, &y_pos, &x_pos);
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
            case ('s'):
                if(save_file(argv[1], program));
                    write_saved();
                break;
            case ('r'):
                write_replace_select();
                break;
            default:
                break;
        }

        refresh();
    }
    delete_program(program);
    finish();
}
