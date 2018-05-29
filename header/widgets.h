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

void write_program(program_t* program, uint32_t start_line) {
    move(0, 0);
    attrset(COLOR_PAIR(7));
    for(size_t i = start_line; i < program->line_amount && (i - start_line) < LINES - 1; i++){
        char line_num_as_hex[32];
        sprintf(line_num_as_hex, "%#010x", program->lines[i]->line_num);

        addstr(line_num_as_hex);
        addstr(" ");
        addstr(program->lines[i]->instruction_name);
        addstr(" ");
        for(size_t j = 0; j < program->lines[i]->instruction_args_amount; j++){
            char arg_as_hex[8];
            sprintf(arg_as_hex, "%#04x", program->lines[i]->instruction_args[j]);
            addstr(arg_as_hex);
            addstr(" ");
        }
        move((int)(i - start_line) + 1, 0);
    }
    attrset(COLOR_PAIR(0));
    refresh();
}

#endif