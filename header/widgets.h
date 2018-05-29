//
// Created by a_mod on 29.05.2018.
//

#ifndef VM_VISUALIZER_WIDGETS_H
#define VM_VISUALIZER_WIDGETS_H

#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <ncurses/curses.h>
#include <locale.h>

#include "program.h"

void write_keymap_line(){
    move(LINES - 1, 0);
    attrset(COLOR_PAIR(8));
    addstr("q(quit)");
    attrset(COLOR_PAIR(0));
    refresh();
}

void write_program(program_t* program) {

}

#endif