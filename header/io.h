//
// Created by a_mod on 29.05.2018.
//

#ifndef VM_VISUALIZER_IO_H
#define VM_VISUALIZER_IO_H

#include "ncurses/curses.h"

#include "widgets.h"

int get_key_press(){
    int x_pos;
    int y_pos;
    getyx(stdscr, y_pos, x_pos);
    chtype old_c = inch();
    int c = getch();
    move(y_pos, x_pos);
    addch(old_c);
    move(y_pos, x_pos);
    return c;
}

void scroll_up(int y_pos, int x_pos, int64_t *fist_displayed_line, program_t *program){
    //Scroll the view up if necessary
    if(y_pos <= 0 && *fist_displayed_line - 1 >= 0){
        clear();
        *fist_displayed_line = *fist_displayed_line - 1;
        write_program(program, (uint32_t)*fist_displayed_line);
        write_keymap_line();
        move(0, 0);
    }else
        move(y_pos -1, x_pos);
}

void scroll_down(int y_pos, int x_pos, int64_t *scroll_pos, program_t *program){
    //Scroll the view down if necessary
    if(y_pos >= LINES - 2){
        clear();
        *scroll_pos = *scroll_pos + 1;
        write_program(program, (uint32_t)(*scroll_pos));
        write_keymap_line();
        move(LINES - 2, x_pos);
    }else
        move(y_pos + 1, x_pos);
}

#endif //VM_VISUALIZER_IO_H
